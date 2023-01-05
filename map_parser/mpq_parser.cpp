//
// Created by zealot on 02.01.2023.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>
#include <bzlib.h>
#include <zlib.h>

#include "mpq_parser.h"
#include "util.h"
#include "abstract_map_file_parser.h"
#include "enviroment_file_map_parser.h"

mpq_parser::mpq_parser(std::ifstream &map) : _map(map) {

}

//00h: char(4) Magic             Indicates that the file is a MoPaQ archive. Must be ASCII "MPQ" 1Ah.
//04h: int32 HeaderSize          Size of the archive header.
//08h: int32 ArchiveSize         Size of the whole archive, including the header. Does not include the strong digital signature,
//if present. This size is used, among other things, for determining the region to hash in computing
//        the digital signature. This field is deprecated in the Burning Crusade MoPaQ format, and the size
//        of the archive is calculated as the size from the beginning of the archive to the end of the
//hash table, block table, or extended block table (whichever is largest).
//0Ch: int16 FormatVersion       MoPaQ format version. MPQAPI will not open archives where this is negative. Known versions:
//0000h                  Original format. HeaderSize should be 20h, and large archives are not supported.
//0001h                  Burning Crusade format. Header size should be 2Ch, and large archives are supported.
//0Eh: int8 SectorSizeShift      Power of two exponent specifying the number of 512-byte disk sectors in each logical sector
//        in the archive. The size of each logical sector in the archive is 512 * 2^SectorSizeShift.
//Bugs in the Storm library dictate that this should always be 3 (4096 byte sectors).
//10h: int32 HashTableOffset     Offset to the beginning of the hash table, relative to the beginning of the archive.
//14h: int32 BlockTableOffset    Offset to the beginning of the block table, relative to the beginning of the archive.
//18h: int32 HashTableEntries    Number of entries in the hash table. Must be a power of two, and must be less than 2^16
//for the original MoPaQ format, or less than 2^20 for the Burning Crusade format.
//1Ch: int32 BlockTableEntries   Number of entries in the block table.
void mpq_parser::parse() {
    parse_header();
    int header_size = read_int_le(_map);
    std::cout << "header size " << header_size << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int archive_size = read_int_le(_map);
    std::cout << "archive size " << archive_size << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int16_t format_version = read_int_16_le(_map);
    std::cout << "format version " << format_version << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int16_t shift = read_int_16_le(_map);
    unsigned sector_size = 0x200 << shift;
    std::cout << "SectorSizeShift " << shift << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int hash_table_offset = read_int_le(_map);
    std::cout << "hash table offset " << hash_table_offset << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int block_table_offset = read_int_le(_map);
    std::cout << "block table offset " << block_table_offset << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int hash_table_entries = read_int_le(_map);
    std::cout << "hash table entries " << hash_table_entries << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int block_table_entries = read_int_le(_map);
    std::cout << "block table entries " << block_table_entries << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    block_table_entries_container = std::vector<block_table_entry>(block_table_entries);
    hash_table_entry_container = std::vector<hash_table_entry>(hash_table_entries);
    _map.seekg(HEADER_SIZE + block_table_offset, std::ios_base::beg);
    std::cout << "file pos now  " << _map.tellg() << std::endl;
    std::cout << "prepare crypt table " << std::endl;
    prepareCryptTable();

    char *block = new char[block_table_entries * 16];
    _map.read(block, block_table_entries * 16);
//    ConvertUInt32Buffer(block,block_table_entries * 64);
    DecryptMpqBlock(block, block_table_entries * 16, MPQ_KEY_BLOCK_TABLE);
//    ConvertUInt32Buffer(block,block_table_entries * 64);
//    for (int i = 0; i < block_table_entries; i++) {
//        read_block(block, i);
//    }
    for (int i = 0; i < block_table_entries; i++) {
        block_table_entries_container[i] = read_block_entry(block, i);
    }
    _map.seekg(HEADER_SIZE + hash_table_offset, std::ios_base::beg);
    std::cout << "file pos now  " << _map.tellg() << std::endl;
    char *hashes = new char[hash_table_entries * 16];
    _map.read(hashes, hash_table_entries * 16);
    DecryptMpqBlock(hashes, hash_table_entries * 16, MPQ_KEY_HASH_TABLE);
    for (int i = 0; i < hash_table_entries; i++) {
        hash_table_entry_container[i] = read_hash_entry(block, i);
    }
    unsigned list_file_index = get_block_index("(listfile)");
    std::string listfiles = "(listfile)";
    block_table_entry list_file_entry = block_table_entries_container[list_file_index];
    read_block(block, 14, "(listfile)");
    unsigned offset = calculate_offset(list_file_entry);
    _map.seekg(offset, std::ios_base::beg);
    std::cout << "file pos now  " << _map.tellg() << std::endl;
    unsigned byte_to_read = calculate_byte_to_read(list_file_entry);
    char *lf = new char[byte_to_read];

    _map.read(lf, byte_to_read);
    unsigned dwFileKey = calculate_key(listfiles.c_str(),
                                       list_file_entry);
    DecryptMpqBlock(lf, byte_to_read, dwFileKey);
    char *lf_ = new char[list_file_entry.file_size];
    unsigned char compression = *lf++;
    decompress_zlib(lf, lf_, list_file_entry.file_comp_size - 1, list_file_entry.file_size);
    std::string res_files(lf_, list_file_entry.file_size);
    std::vector<std::string> detected_files = parse_list_file(res_files);
    for (std::string s: detected_files) {
        unsigned file_index = get_block_index(s.c_str());
        block_table_entry entry = block_table_entries_container[file_index];
        std::cout << "found file " << s << " with size " << entry.file_size << std::endl;
        abstract_map_file_parser* parser = get_parser(s, entry, _map);
        if (parser) {
            std::cout << "for file " << s << " found parser" << std::endl;
            parser->parse();
        }
        delete parser;
    }
    delete[] lf_;
    delete[] block;
    delete[] hashes;
}

void mpq_parser::DecryptMpqBlock(void *pvDataBlock, unsigned dwLength, unsigned dwKey1) {
    unsigned *DataPointer = (unsigned int *) pvDataBlock;
    unsigned dwValue32;
    unsigned dwKey2 = 0xEEEEEEEE;
    unsigned MPQ_HASH_KEY2_MIX = 0x400;
    // Round to DWORDs
    dwLength >>= 2;
//    dwKey2 += cryptTable[MPQ_HASH_KEY2_MIX + (dwKey1 & 0xFF)];
    // We need different approach on non-aligned buffers
    if ((((size_t) (DataPointer) & 0x03) == 0)) {
        for (unsigned i = 0; i < dwLength; i++) {
            // Modify the second key
            dwKey2 += cryptTable[MPQ_HASH_KEY2_MIX + (dwKey1 & 0xFF)];
//            unsigned encrypted =  DataPointer[i];

            // We can use 32-bit approach, when the buffer is aligned
//            unsigned decrypted = dwValue32 = DataPointer[i] ^ (dwKey1 + dwKey2);
//            std::cout << "value  " << encrypted << "decrypted as" << decrypted <<  std::endl;
            DataPointer[i] = dwValue32 = DataPointer[i] ^ (dwKey1 + dwKey2);

            dwKey1 = ((~dwKey1 << 0x15) + 0x11111111) | (dwKey1 >> 0x0B);
            dwKey2 = dwValue32 + dwKey2 + (dwKey2 << 5) + 3;
        }
    } else {
        throw std::runtime_error("unsupported for now");
//        for(long i = 0; i < dwLength; i++)
//        {
//            // Modify the second key
//            dwKey2 += cryptTable[MPQ_HASH_KEY2_MIX + (dwKey1 & 0xFF)];
//
//            // The data are unaligned. Make sure we don't cause data misalignment error
//            dwValue32 = DecryptUInt32Unaligned(DataPointer, i, (dwKey1 + dwKey2));
//
//            dwKey1 = ((~dwKey1 << 0x15) + 0x11111111) | (dwKey1 >> 0x0B);
//            dwKey2 = dwValue32 + dwKey2 + (dwKey2 << 5) + 3;
//        }
    }
}

void mpq_parser::DecryptBlock(void *block, int length, unsigned int key) {
    unsigned int seed = 0xEEEEEEEE;
    unsigned int ch;
    unsigned int *castBlock = (unsigned int *) block;

    // Round to longs
//    length >>= 2;

    while (length-- > 0) {
        seed += cryptTable[0x400 + (key & 0xFF)];
        ch = *castBlock ^ (key + seed);

        key = ((~key << 0x15) + 0x11111111) | (key >> 0x0B);
        seed = ch + seed + (seed << 5) + 3;
        *castBlock++ = ch;
    }
}

void mpq_parser::DecryptData(void *lpbyBuffer, unsigned long dwLength, unsigned long dwKey) {
    assert(lpbyBuffer);

    unsigned long *lpdwBuffer = (unsigned long *) lpbyBuffer;
    unsigned long seed = 0xEEEEEEEEL;
    unsigned long ch;

    dwLength /= sizeof(unsigned long);

    while (dwLength-- > 0) {
        seed += cryptTable[0x400 + (dwKey & 0xFF)];
        ch = *lpdwBuffer ^ (dwKey + seed);

        dwKey = ((~dwKey << 0x15) + 0x11111111L) | (dwKey >> 0x0B);
        seed = ch + seed + (seed << 5) + 3;

        *lpdwBuffer++ = ch;
    }
}

//
// Note: Implementation of this function in WorldEdit.exe and storm.dll
// incorrectly treats the character as signed, which leads to the
// a buffer underflow if the character in the file name >= 0x80:
// The following steps happen when *pbKey == 0xBF and dwHashType == 0x0000
// (calculating hash index)
//
// 1) Result of AsciiToUpperTable_Slash[*pbKey++] is sign-extended to 0xffffffbf
// 2) The "ch" is added to dwHashType (0xffffffbf + 0x0000 => 0xffffffbf)
// 3) The result is used as index to the StormBuffer table,
// thus dereferences a random value BEFORE the begin of StormBuffer.
//
// As result, MPQs containing files with non-ANSI characters will not work between
// various game versions and localizations. Even WorldEdit, after importing a file
// with Korean characters in the name, cannot open the file back.
//
unsigned mpq_parser::HashString(const char *szFileName, unsigned dwHashType) {
    char *pbKey = (char *) szFileName;
    unsigned dwSeed1 = 0x7FED7FED;
    unsigned dwSeed2 = 0xEEEEEEEE;
    unsigned ch;

    while (*pbKey != 0) {
        // Convert the input character to uppercase
        // Convert slash (0x2F) to backslash (0x5C)
        ch = AsciiToUpperTable[*pbKey++];

        dwSeed1 = cryptTable[dwHashType + ch] ^ (dwSeed1 + dwSeed2);
        dwSeed2 = ch + dwSeed1 + dwSeed2 + (dwSeed2 << 5) + 3;
    }

    return dwSeed1;
}

void mpq_parser::prepareCryptTable() {
    unsigned seed = 0x00100001, index1 = 0, index2 = 0, i;

    for (index1 = 0; index1 < 0x100; index1++) {
        for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100) {
            unsigned temp1, temp2;

            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp1 = (seed & 0xFFFF) << 0x10;

            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp2 = (seed & 0xFFFF);

            cryptTable[index2] = (temp1 | temp2);
        }
    }
}

void mpq_parser::parse_header() {
    char *format = new char[4];
    _map.read(format, 4);
    std::cout << "header is  " << std::string(format, 4) << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    delete[] format;
}

//00h: int32 BlockOffset   Offset of the beginning of the block, relative to the beginning of the archive.
//04h: int32 BlockSize     Size of the block in the archive.
//08h: int32 FileSize      Size of the file data stored in the block. Only valid if the block is a file; otherwise
//        meaningless, and should be 0. If the file is compressed, this is the size of the uncompressed
//file data.
//0Ch: int32 Flags         Bit mask of the flags for the block. The following values are conclusively identified:
//80000000h        Block is a file, and follows the file data format; otherwise, block is free space or unused.
//If the block is not a file, all other flags should be cleared, and FileSize should be 0.
//04000000h	 File has checksums for each sector (explained in the File Data section). Ignored if file is not
//compressed or imploded.
//02000000h        File is a deletion marker, indicating that the file no longer exists. This is used to allow
//        patch archives to delete files present in lower-priority archives in the search chain.
//01000000h        File is stored as a single unit, rather than split into sectors.
//00020000h        The file's encryption key is adjusted by the block offset and file size (explained in detail in the
//File Data section). File must be encrypted.
//00010000h        File is encrypted.
//00000200h        File is compressed. File cannot be imploded.
//00000100h        File is imploded. File cannot be compressed.
void mpq_parser::read_block(char *block, int i) {
    unsigned *casted = (unsigned *) block;
    unsigned block_offset = casted[0 + (4 * i)];
    std::cout << "block offset for " << i << " block  is " << block_offset << std::endl;
    unsigned compressed_file_size = casted[1 + (4 * i)];
    std::cout << "compressed_file_size " << i << " block  is " << compressed_file_size << std::endl;
    unsigned file_size = casted[2 + (4 * i)];
    std::cout << "file_size for " << i << " block  is " << file_size << std::endl;
//    unsigned block_offset = read_int_le(block+ (16 * i));
//    std::cout << "block offset for " << i << " block  is " << block_offset << std::endl;
//    unsigned block_size = read_int_le(block + 4+ (16 * i));
//    std::cout << "block size for " << i << " block  is " << block_size << std::endl;
//    unsigned file_size = read_int_le(block + 8+ (16 * i));
//    std::cout << "file size for " << i << " block  is " << file_size << std::endl;
    unsigned file_flag = casted[3 + (4 * i)];
    if (file_flag & 0x80000000) {
        std::cout << "block " << i << " is file" << std::endl;
    }
    if (file_flag & 0x04000000) {
        std::cout << "block " << i << " has checksum" << std::endl;
    }
    if (file_flag & 0x02000000) {
        std::cout << "block " << i << " has delerted" << std::endl;
    }
    if (file_flag & 0x01000000) {
        std::cout << "block " << i << " stored in single unit" << std::endl;
    }
    if (file_flag & 0x00020000) {
        std::cout << "block " << i << " stored in single unit" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
}

block_table_entry mpq_parser::read_block_entry(char *data, int i) {
    block_table_entry blockTableEntry;
    unsigned *casted = (unsigned *) data;
    blockTableEntry.file_pos = casted[0 + (4 * i)];
    blockTableEntry.file_comp_size = casted[1 + (4 * i)];
    blockTableEntry.file_size = casted[2 + (4 * i)];
    unsigned file_flag = casted[3 + (4 * i)];
    blockTableEntry.is_file_compress = file_flag & 0x00000200;
    blockTableEntry.is_file_deleted = file_flag & 0x02000000;
    blockTableEntry.is_file_encrypted = file_flag & 0x00010000;
    blockTableEntry.is_file_exists = file_flag & 0x80000000;
    blockTableEntry.is_file_implode = file_flag & 0x00000100;
    blockTableEntry.is_file_patch = file_flag & 0x00100000;
    blockTableEntry.is_file_single_unit = file_flag & 0x01000000;
    blockTableEntry.is_file_start_crc_header = file_flag & 0x04000000;
    blockTableEntry.is_file_need_fix_key = file_flag & 0x00020000;
    return blockTableEntry;
}

hash_table_entry mpq_parser::read_hash_entry(char *data, int i) {
    hash_table_entry hashTableEntry;
    unsigned *casted = (unsigned *) data;
    hashTableEntry.name1 = casted[0 + (4 * i)];
    hashTableEntry.name2 = casted[1 + (4 * i)];
    hashTableEntry.block_index = casted[3 + (4 * i)];
    return hashTableEntry;
}


void mpq_parser::read_block(char *block, int i, std::string s) {
    unsigned *casted = (unsigned *) block;
    unsigned block_offset = casted[0 + (4 * i)];
    std::cout << "block offset for " << s << " block  is " << block_offset << std::endl;
    unsigned compressed_file_size = casted[1 + (4 * i)];
    std::cout << "compressed_file_size " << s << " block  is " << compressed_file_size << std::endl;
    unsigned file_size = casted[2 + (4 * i)];
    std::cout << "file_size for " << i << " block  is " << file_size << std::endl;
//    unsigned block_offset = read_int_le(block+ (16 * i));
//    std::cout << "block offset for " << i << " block  is " << block_offset << std::endl;
//    unsigned block_size = read_int_le(block + 4+ (16 * i));
//    std::cout << "block size for " << i << " block  is " << block_size << std::endl;
//    unsigned file_size = read_int_le(block + 8+ (16 * i));
//    std::cout << "file size for " << i << " block  is " << file_size << std::endl;
    unsigned file_flag = casted[3 + (4 * i)];
    if (file_flag & 0x80000000) {
        std::cout << "block " << s << " is file" << std::endl;
    }
    if (file_flag & 0x00000100) {
        std::cout << "block " << s << "  is compressed using PKWARE Data compression library" << std::endl;
    }
    if (file_flag & 0x00000200) {
        std::cout << "block " << s << "   is compressed using combination of compression methods" << std::endl;
    }
    if (file_flag & 0x04000000) {
        std::cout << "block " << s << " has checksum" << std::endl;
    }
    if (file_flag & 0x02000000) {
        std::cout << "block " << s << " has deleted" << std::endl;
    }
    if (file_flag & 0x01000000) {
        std::cout << "block " << s << " stored in single unit" << std::endl;
    }
    if (file_flag & 0x00020000) {
        std::cout << "block " << s << " stored in single unit" << std::endl;
    }
    if (file_flag & 0x04000000) {
        std::cout << "block " << s << " has crc in this" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}


void mpq_parser::read_block_(char *block, int i) {
    unsigned *casted = (unsigned *) block;
    unsigned block_offset = casted[0];
    std::cout << "block offset for " << i << " block  is " << block_offset << std::endl;
    unsigned block_size = casted[1];
    std::cout << "block size for " << i << " block  is " << block_size << std::endl;
//      block_offset = read_int_le(block  );
//    std::cout << "block offset for " << i << " block  is " << block_offset << std::endl;
//      block_size = read_int_le(block + 4);
//    std::cout << "block size for " << i << " block  is " << block_size << std::endl;
//    unsigned file_size = read_int_le(block + 8);
//    std::cout << "file size for " << i << " block  is " << file_size << std::endl;
//    unsigned file_flag = read_int_le(block + 12);
//    if (file_flag & 0x80000000) {
//        std::cout << "block " << i << " is file" << std::endl;
//    }
//    if (file_flag & 0x04000000) {
//        std::cout << "block " << i << " has checksum" << std::endl;
//    }
//    if (file_flag & 0x02000000) {
//        std::cout << "block " << i << " has delerted" << std::endl;
//    }
//    if (file_flag & 0x01000000) {
//        std::cout << "block " << i << " stored in single unit" << std::endl;
//    }
//    if (file_flag & 0x00020000) {
//        std::cout << "block " << i << " stored in single unit" << std::endl;
//    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
// The hash of the full file name (part A)
//DWORD dwName1;

// The hash of the full file name (part B)
//DWORD dwName2;

// The language of the file. This is a Windows LANGID data type, and uses the same values.
// 0 indicates the default language (American English), or that the file is language-neutral.
//USHORT lcLocale;

// The platform the file is used for. 0 indicates the default platform.
// No other values have been observed.
//USHORT wPlatform;

// If the hash table entry is valid, this is the index into the block table of the file.
// Otherwise, one of the following two values:
//  - FFFFFFFFh: Hash table entry is empty, and has always been empty.
//               Terminates searches for a given file.
//  - FFFFFFFEh: Hash table entry is empty, but was valid at some point (a deleted file).
//               Does not terminate searches for a given file.
//DWORD dwBlockIndex;
void mpq_parser::read_hash_entry(int i) {
    unsigned hash_a = read_int_le(_map);
    std::cout << "hash a " << i << " block  is " << hash_a << std::endl;
    unsigned hash_b = read_int_le(_map);
    std::cout << "hash b " << i << " block  is " << hash_b << std::endl;
    uint16_t platform = read_int_16_le(_map);
    std::cout << " platform " << i << " block  is " << platform << std::endl;
    unsigned char language = _map.get();
    std::cout << " language " << i << " block  is " << language << std::endl;
    _map.get();
    unsigned file_block_index = read_int_le(_map);
    std::cout << "file_block_index  " << i << " block  is " << file_block_index << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void mpq_parser::decompress_bzlib(char *in, char *out, unsigned av_in, unsigned av_out) {
    bz_stream strm;
    int nResult;

    // Initialize the BZIP2 decompression
    strm.next_in = in;
    strm.avail_in = av_in;
    strm.next_out = out;
    strm.avail_out = av_out;
    strm.bzalloc = NULL;
    strm.bzfree = NULL;
    strm.opaque = NULL;

    // Initialize decompression
    if ((nResult = BZ2_bzDecompressInit(&strm, 0, 0)) == BZ_OK) {
        // Perform the decompression
        nResult = BZ2_bzDecompress(&strm);
//        *pcbOutBuffer = strm.total_out_lo32;
        BZ2_bzDecompressEnd(&strm);
    }
    if (nResult >= BZ_OK) {
        return;
    }
    throw std::runtime_error("coud nt decompress");
}

void mpq_parser::decompress_zlib(char *in, char *out, unsigned int av_in, unsigned int av_out) {
    z_stream z;                        // Stream information for zlib
    int nResult;

    // Fill the stream structure for zlib
    z.next_in = (Bytef *) in;
    z.avail_in = (uInt) av_in;
    z.total_in = av_in;
    z.next_out = (Bytef *) out;
    z.avail_out = av_out;
    z.total_out = 0;
    z.zalloc = NULL;
    z.zfree = NULL;

    // Initialize the decompression structure. Storm.dll uses zlib version 1.1.3
    if ((nResult = inflateInit(&z)) == Z_OK) {
        // Call zlib to decompress the data
        nResult = inflate(&z, Z_FINISH);
//        *pcbOutBuffer = z.total_out;
        inflateEnd(&z);
    }

    if (nResult >= Z_OK) {
        return;
    }
    throw std::runtime_error("coud nt decompress");
}

unsigned int mpq_parser::get_block_index(const char *file_name) {
    unsigned dwName1 = HashString(file_name, MPQ_HASH_TYPE_A);
    unsigned dwName2 = HashString(file_name, MPQ_HASH_TYPE_B);
    int block_index = -1;
    for (hash_table_entry hash: hash_table_entry_container) {
        if (hash.name1 == dwName1 && hash.name2 == dwName2) {
            return hash.block_index;
        }
    }
    throw std::runtime_error("could not find file");
}

unsigned int mpq_parser::calculate_offset(const block_table_entry &entry) {
    unsigned res = HEADER_SIZE;
    res += entry.file_pos;
//    if (entry.is_file_start_crc_header) {
    res += 8;// todo offset before sector
//    }
    return res;
}

unsigned int mpq_parser::calculate_byte_to_read(const block_table_entry &entry) {
    unsigned res = entry.is_file_compress ? entry.file_comp_size : entry.file_size;
//    if (entry.is_file_start_crc_header) {
    res -= 8;// ignore crc for now
//    }
    return res;
}

unsigned int mpq_parser::calculate_key(const char *str, const block_table_entry &entry) {
    unsigned dwFileKey = HashString(str, MPQ_HASH_FILE_KEY);//758152009
    dwFileKey = (dwFileKey + entry.file_pos) ^ entry.file_size;
    return dwFileKey;
}

std::vector<std::string> mpq_parser::parse_list_file(std::string s) {
    std::vector<std::string> res;
    std::string delimiter = "\r\n";

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        res.push_back(token);
        s.erase(0, pos + delimiter.length());
    }

    return res;
}

abstract_map_file_parser *mpq_parser::get_parser(std::string s, block_table_entry& bp, std::ifstream& _map) {
    if (s == "war3map.w3e" )
            return new enviroment_file_map_parser(bp, _map);
    return nullptr;
}


