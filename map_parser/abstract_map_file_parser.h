//
// Created by zealot on 05.01.2023.
//

#ifndef WAR3_MAP_PARSER_ABSTRACT_MAP_FILE_PARSER_H
#define WAR3_MAP_PARSER_ABSTRACT_MAP_FILE_PARSER_H


#include <fstream>
#include "../data_objects/block_table_entry.h"

class abstract_map_file_parser {
public:
    virtual void parse() = 0;
    abstract_map_file_parser(block_table_entry& entry,std::ifstream& map) : bp(entry), _map(map) {}
protected:
    block_table_entry& bp;
    std::ifstream & _map;
};


#endif //WAR3_MAP_PARSER_ABSTRACT_MAP_FILE_PARSER_H
