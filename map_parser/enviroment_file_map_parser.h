//
// Created by zealot on 05.01.2023.
//

#ifndef WAR3_MAP_PARSER_ENVIROMENT_FILE_MAP_PARSER_H
#define WAR3_MAP_PARSER_ENVIROMENT_FILE_MAP_PARSER_H


#include "abstract_map_file_parser.h"

class enviroment_file_map_parser : public abstract_map_file_parser{
public:

    enviroment_file_map_parser(block_table_entry &entry, std::ifstream &fstream) : abstract_map_file_parser(entry, fstream) {};

    void parse() override;
};


#endif //WAR3_MAP_PARSER_ENVIROMENT_FILE_MAP_PARSER_H
