//
// Created by zealot on 05.01.2023.
//

#ifndef WAR3_MAP_PARSER_ENVIROMENT_FILE_MAP_PARSER_H
#define WAR3_MAP_PARSER_ENVIROMENT_FILE_MAP_PARSER_H

#include <image.hpp>
#include "abstract_map_file_parser.h"
#include "../data_objects/war3_inner_object/tileset_entry.h"
#include "../data_objects/parser_inner_object/Vertex.h"
#include "../file_utils/csv_parser.h"
#include "../data_objects/parser_inner_object/TexCoord.h"
#include "../file_utils/terrain_csv_parser.h"

class enviroment_file_map_parser : public abstract_map_file_parser {
public:
    virtual ~enviroment_file_map_parser();

    enviroment_file_map_parser(block_table_entry &entry, std::ifstream &fstream) : abstract_map_file_parser(entry,
                                                                                                            fstream)
                                                                                   {};

    virtual void parse() override;

    virtual void aggregate() override;

    void read_block(const std::vector<unsigned int> &offset_table, int i);

private:
    unsigned char *unpacked_data;
    std::vector<tileset_entry> tilesets;
    std::vector<Vertex> verticies;
    std::vector<TexCoord> texcoords;
    std::vector<unsigned> indicies;

    csv_parser<terrain_entry>* parser = new terrain_csv_parser("/home/zealot/CLionProjects/War3_Map_Parser/assets/TerrainArt/Terrain.csv");

    unsigned char main_tileset;
    unsigned int custom_tilesets_flag;
    char **tileset_table;
    std::unordered_map<std::string, png::image<png::rgb_pixel >> tileset_textures;
    png::image<png::rgb_pixel > result_texture;
    char **cliff_tileset_table;
    unsigned int x;
    unsigned int y;
    float center_offset_x;
    float center_offset_y;
    unsigned int a;
    unsigned int b;
    unsigned long size;

    void generate_mesh();

    void add_triangle(const unsigned &x1, const unsigned &x2, const unsigned &x3);

    void write_mesh();
};


#endif //WAR3_MAP_PARSER_ENVIROMENT_FILE_MAP_PARSER_H
