//
// Created by zealot on 01.02.2023.
//

#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>
#include "doodads_parser.h"
#include "../decompressors/abstract_decompressor.h"
#include "util.h"

void doodads_parser::parse() {
    unsigned bytes_to_read = bp.is_file_compress ? bp.file_comp_size : bp.file_size;
    char *data = new char[bytes_to_read];
    _map.seekg(512 + bp.file_pos); //todo remove hardcode
    _map.read(data, bytes_to_read);
    if (bp.is_file_encrypted) {
        std::cout << "file with enviromet data ncrypted" << std::endl;
    }
    if (bp.is_file_compress) {
        std::cout << " file  with enviromet data  compressed" << std::endl;
    }
    unsigned *casted = (unsigned *) (data);
    std::vector<unsigned> offset_table;//todo move to abstaract parser
    unsigned _offset = *casted++;
    while (_offset != bp.file_comp_size) {
        offset_table.push_back(_offset);
        _offset = *casted++;
    }
    unsigned sector_size = 4096;//todo remove hardcode
    unpacked_data = new unsigned char[sector_size *
                                      offset_table.size()];//todo выделять памяти сколько требуется, а не больше
    for (int i = 0; i < offset_table.size() - 1; i++) {
        read_block(offset_table, i);
    }
    delete[] data;
    aggregate();
}

void doodads_parser::aggregate() {
    std::cout << "start aggreagte doodads data" << std::endl;
    unsigned int count = 0;
    std::string w3e = std::string((char *) unpacked_data, 4);
    unpacked_data += 4;
    count += 4;
    assert (w3e == "W3do!");
    unsigned version = read_int_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    assert (version == 8);
    unsigned subversion = read_int_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    numbers_of_trees = read_int_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    std::cout << numbers_of_trees << "trees" << std::endl;
    for (int i = 0; i < numbers_of_trees; i++) {
        tree_entry tr;
        tr.x = read_float_le(unpacked_data);
        count += 4;
        tr.y = read_float_le(unpacked_data);
        count += 4;
        tr.z = read_float_le(unpacked_data);
        count += 4;
        tr.angle = read_float_le(unpacked_data);
        count += 4;
        tr.scale_x = read_float_le(unpacked_data);
        count += 4;
        tr.scale_y = read_float_le(unpacked_data);
        count += 4;
        tr.scale_z = read_float_le(unpacked_data);
        count += 4;
        unsigned flags = *unpacked_data++;
        count++;
        tr.visible = flags > 0;
        tr.solid = flags > 1;
        tr.health = *unpacked_data++;
        count++;
        tr.item_id = read_int_le(unpacked_data);
        count += 4;
        tr.item_count = read_int_le(unpacked_data);
        count += 4;
        tr.id = read_int_le(unpacked_data);
        count += 4;
    }
    unpacked_data -= count;// for safe deleteing
}

void doodads_parser::read_block(const std::vector<unsigned int> &offset_table, int i) {
    unsigned bytes_to_read = offset_table[i + 1] -
                             offset_table[i]; //(relative to the beginning of the file in the MPQ is stored at the beginning of the file data
    _map.seekg(512 + bp.file_pos + offset_table[i]);
    char *data = new char[bytes_to_read];
    _map.read(data, bytes_to_read);
    unsigned char compression = *data++;
    abstract_decompressor *decompressor = abstract_decompressor::get_decompressor(compression);
    char *out_data = new char[4096];//todo remove hardcode
    decompressor->decompress(data, out_data, bytes_to_read--, 4096);
    memcpy(unpacked_data + 4096 * i, out_data, 4096);
    delete[] out_data;
    delete decompressor;
}

doodads_parser::~doodads_parser() {
    delete[] unpacked_data;
}
