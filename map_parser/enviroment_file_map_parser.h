//
// Created by zealot on 05.01.2023.
//

#ifndef WAR3_MAP_PARSER_ENVIROMENT_FILE_MAP_PARSER_H
#define WAR3_MAP_PARSER_ENVIROMENT_FILE_MAP_PARSER_H


#include "abstract_map_file_parser.h"
#include "../data_objects/war3_inner_object/tileset_entry.h"

class enviroment_file_map_parser : public abstract_map_file_parser{
public:
    virtual ~enviroment_file_map_parser();
    enviroment_file_map_parser(block_table_entry &entry, std::ifstream &fstream) : abstract_map_file_parser(entry, fstream) {};

    void parse() override;

    void aggregate() override;

    void read_block(const std::vector<unsigned int> &offset_table, int i);
private:
    unsigned char* unpacked_data;
    std::vector<tileset_entry> tilesets;

    unsigned char main_tileset;
    unsigned int custom_tilesets_flag;
    char** tileset_table;
    char** cliff_tileset_table;
    unsigned int x;
    unsigned int y;
    float center_offset_x;
    float center_offset_y;
    unsigned int a;
    unsigned int b;
    unsigned long size;
};


#endif //WAR3_MAP_PARSER_ENVIROMENT_FILE_MAP_PARSER_H
