//
// Created by zealot on 05.01.2023.
//

#ifndef WAR3_MAP_PARSER_HASH_TABLE_ENTRY_H
#define WAR3_MAP_PARSER_HASH_TABLE_ENTRY_H
//DWORD dwName1;
//
//// The hash of the full file name (part B)
//DWORD dwName2;
//
//// The language of the file. This is a Windows LANGID data type, and uses the same values.
//// 0 indicates the default language (American English), or that the file is language-neutral.
//USHORT lcLocale;
//
//// The platform the file is used for. 0 indicates the default platform.
//// No other values have been observed.
//USHORT wPlatform;
//
//// If the hash table entry is valid, this is the index into the block table of the file.
//// Otherwise, one of the following two values:
////  - FFFFFFFFh: Hash table entry is empty, and has always been empty.
////               Terminates searches for a given file.
////  - FFFFFFFEh: Hash table entry is empty, but was valid at some point (a deleted file).
////               Does not terminate searches for a given file.
//DWORD dwBlockIndex;

#include <cstdint>

class hash_table_entry {
    friend class mpq_parser;
private:
    unsigned name1;
    unsigned name2;
    uint16_t locale;
    uint16_t platform;
    unsigned block_index;

};


#endif //WAR3_MAP_PARSER_HASH_TABLE_ENTRY_H
