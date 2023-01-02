//
// Created by zealot on 02.01.2023.
//

#include <iostream>
#include <thread>
#include <chrono>
#include "mpq_parser.h"
#include "util.h"

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
    std::cout << "format version " << archive_size << std::endl;
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
    _map.seekg(512 + block_table_offset);
    for (int i = 0; i < block_table_entries; i++) {
        read_block(i);
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
void mpq_parser::read_block(int i) {
    int block_offset = read_int_le(_map);
    std::cout << "block offset for " << i << " block  is" << block_offset << std::endl;
    int block_size = read_int_le(_map);
    std::cout << "block size for " << i << " block  is" << block_size << std::endl;
    int file_size = read_int_le(_map);
    std::cout << "file size for " << i << " block  is" << file_size << std::endl;
    int file_flag = read_int_le(_map);
    std::cout << "file size for " << i << " block  is" << file_size << std::endl;
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
//    if (file_flag & 0x00020000) {
//        std::cout << "block " << i << " stored in single unit" << std::endl;
//    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
