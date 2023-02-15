//
// Created by zealot on 01.02.2023.
//

#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>
#include <sstream>
#include <filesystem>
#include "doodads_parser.h"
#include "../decompressors/abstract_decompressor.h"
#include "util.h"
#include "../data_objects/war3_inner_object/special_doodads_entry.h"
#include "../mdx_parser/mdx_parser.h"

void doodads_parser::parse() {
    _doodads_csv_parser->parse();
    _destructable_unit_parser->parse();
    abstract_map_file_parser::parse();
}

void doodads_parser::aggregate() {
    std::cout << "start aggreagte doodads data" << std::endl;
    std::string w3e = std::string(unpacked_data, 4);
    unpacked_data += 4;
    assert (w3e == "W3do");
    unsigned version = read_int_le(unpacked_data);
    assert (version == 7 || version == 8);
    unsigned subversion = read_int_le(unpacked_data);
    numbers_of_trees = read_int_le(unpacked_data);
    std::cout << numbers_of_trees << " trees" << std::endl;
    for (int i = 0; i < numbers_of_trees; i++) {
        tree_entry tr;
        tr.tree_id = std::string((char *) unpacked_data, 4);
        unpacked_data +=4;
        tr.variation = read_int_le(unpacked_data);
        tr.x = read_float_le(unpacked_data);
        tr.y = read_float_le(unpacked_data);
        tr.z = read_float_le(unpacked_data);
        tr.angle = read_float_le(unpacked_data);
        tr.scale_x = read_float_le(unpacked_data);
        tr.scale_y = read_float_le(unpacked_data);
        tr.scale_z = read_float_le(unpacked_data);
        unsigned flags = *unpacked_data++;
        tr.visible = flags > 0;
        tr.solid = flags > 1;
        tr.health = *unpacked_data++;
        if (version == 8) {
            tr.item_id = read_int_le(unpacked_data);
            tr.item_count = read_int_le(unpacked_data);
        }
        tr.id = read_int_le(unpacked_data);
        std::stringstream ss;
        if (_destructable_unit_parser->has(tr.tree_id)) {
            destructable_unit_entry &de = _destructable_unit_parser->get(tr.tree_id);

            ss << _destructable_unit_parser->fix_file_separator(de.dir) << std::filesystem::path::preferred_separator
               << de.file;

            if (de.num_var != 1) {
                ss << std::filesystem::path::preferred_separator << de.file;
                ss << tr.variation;
            }
        } else {
            doodads_entry &de = _doodads_csv_parser->get(tr.tree_id);
            ss << _destructable_unit_parser->fix_file_separator(de.dir) << std::filesystem::path::preferred_separator
               << de.file;
            if (de.num_var != 1) {
                ss << std::filesystem::path::preferred_separator << de.file;
                ss << tr.variation;
            }
        }

        ss << ".mdx";
        trees.push_back(tr);
    }
    mdx_parser _mdx_parser(
            "/home/zealot/CLionProjects/War3_Map_Parser/assets/Doodads/Terrain/LordaeronTree/LordaeronTree0.mdx");
    _mdx_parser.parse();
    int format = read_int_le(unpacked_data);
    assert (format == 0);
    special_doodads = read_int_le(unpacked_data);
    std::cout << special_doodads << " doods" << std::endl;
    for (int i = 0; i < special_doodads; i++) {
        special_doodads_entry sde;
        sde.id = std::string((char *) unpacked_data, 4);
        unpacked_data += 4;
        sde.Z = read_int_le(unpacked_data);
        sde.X = read_int_le(unpacked_data);
        sde.Y = read_int_le(unpacked_data);
    }
    unpacked_data -= bp.file_size;// for safe deleteing
}


doodads_parser::~doodads_parser() {

    delete _doodads_csv_parser;
    delete _destructable_unit_parser;
}


