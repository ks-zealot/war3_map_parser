//
// Created by zealot on 05.01.2023.
//

#ifndef WAR3_MAP_PARSER_ABSTRACT_MAP_FILE_PARSER_H
#define WAR3_MAP_PARSER_ABSTRACT_MAP_FILE_PARSER_H


#include <fstream>
#include "../data_objects/block_table_entry.h"

class abstract_map_file_parser {
public:
    virtual void parse() ;

    virtual void aggregate() = 0;

    abstract_map_file_parser(block_table_entry &entry, std::ifstream &map) : bp(entry), _map(map) {}

    virtual ~abstract_map_file_parser();

protected:
    char *unpacked_data;
    block_table_entry &bp;
    std::ifstream &_map;

    void read_block(const std::vector<unsigned int> &offset_table, int i);
};


#endif //WAR3_MAP_PARSER_ABSTRACT_MAP_FILE_PARSER_H
