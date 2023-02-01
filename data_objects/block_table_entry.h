//
// Created by zealot on 05.01.2023.
//

#ifndef WAR3_MAP_PARSER_BLOCK_TABLE_ENTRY_H
#define WAR3_MAP_PARSER_BLOCK_TABLE_ENTRY_H
//// Offset of the beginning of the file data, relative to the beginning of the archive.
//DWORD dwFilePos;
//
//// Compressed file size
//DWORD dwCSize;
//
//// Size of uncompressed file
//DWORD dwFSize;
//
//// Flags for the file. See the table below for more informations
//DWORD dwFlags;


//Flag name	Value	Meaning
//MPQ_FILE_IMPLODE	0x00000100	File is compressed using PKWARE Data compression library
//        MPQ_FILE_COMPRESS	0x00000200	File is compressed using combination of compression methods
//        MPQ_FILE_ENCRYPTED	0x00010000	The file is encrypted
//MPQ_FILE_FIX_KEY	0x00020000	The decryption key for the file is altered according to the position of the file in the archive
//MPQ_FILE_PATCH_FILE	0x00100000	The file contains incremental patch for an existing file in base MPQ
//MPQ_FILE_SINGLE_UNIT	0x01000000	Instead of being divided to 0x1000-bytes blocks, the file is stored as single unit
//MPQ_FILE_DELETE_MARKER	0x02000000	File is a deletion marker, indicating that the file no longer exists. This is used to allow patch archives to delete files present in lower-priority archives in the search chain. The file usually has length of 0 or 1 byte and its name is a hash
//MPQ_FILE_SECTOR_CRC	0x04000000	File has checksums for each sector (explained in the File Data section). Ignored if file is not compressed or imploded.
//MPQ_FILE_EXISTS	0x80000000	Set if file exists, reset when the file was deleted

class block_table_entry {
    friend class mpq_parser;

    friend class abstract_map_file_parser;

    friend class enviroment_file_map_parser;
    friend class doodads_parser;

private:
    unsigned file_pos;
    unsigned file_comp_size;
    unsigned file_size;

    bool is_file_implode;
    bool is_file_compress;
    bool is_file_encrypted;
    bool is_file_need_fix_key;
    bool is_file_patch;
    bool is_file_single_unit;
    bool is_file_deleted;
    bool is_file_start_crc_header;
    bool is_file_exists;

};


#endif //WAR3_MAP_PARSER_BLOCK_TABLE_ENTRY_H
