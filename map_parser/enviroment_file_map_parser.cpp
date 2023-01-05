//
// Created by zealot on 05.01.2023.
//

#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>
#include "enviroment_file_map_parser.h"
#include "../decompressors/abstract_decompressor.h"
#include "util.h"
#include "../data_objects/war3_inner_object/tileset_entry.h"

void enviroment_file_map_parser::parse() {
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
    std::vector<unsigned> offset_table;
    unsigned _offset = *casted++;
    while (_offset != bp.file_comp_size) {
        offset_table.push_back(_offset);
        _offset = *casted++;
    }
    unsigned sector_size = 4096;//todo remove hardcode
    unpacked_data = new unsigned char[sector_size * offset_table.size() ];
    for (int i = 0; i < offset_table.size() - 1; i++) {
        read_block(offset_table, i);
    }
    aggregate();

}

void enviroment_file_map_parser::read_block(const std::vector<unsigned int> &offset_table, int i) {
    unsigned bytes_to_read = offset_table[i + 1] -
                             offset_table[i]; //(relative to the begin of the file in the MPQ is stored at the begin of the file data
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

enviroment_file_map_parser::~enviroment_file_map_parser() {
    delete[] unpacked_data;
    for (int i = 0; i < a; i++) {
        delete[]  tileset_table[i];
    }
    delete[]  tileset_table;
    for (int i = 0; i < b; i++) {
        delete[]  cliff_tileset_table[i];
    }
    delete[]  cliff_tileset_table;
}

void enviroment_file_map_parser::aggregate() {
    std::cout << "start aggreagte enviroment data" << std::endl;
    unsigned int count = 0;
    std::string w3e = std::string((char *) unpacked_data, 4);
    unpacked_data += 4;
    count+= 4;
    assert (w3e == "W3E!");
    unsigned version = read_int_le(unpacked_data);
    unpacked_data += 4;
    count+= 4;
    assert(version == 11);
    main_tileset = *unpacked_data++;
    count++;
    custom_tilesets_flag = read_int_le(unpacked_data);
    unpacked_data += 4;
    count+= 4;
    a = read_int_le(unpacked_data);
    unpacked_data += 4;
    count+= 4;
    tileset_table = new char *[a];
    for (int i = 0; i < a; i++) {
        tileset_table[i] = new char[4];
        memcpy(tileset_table[i], unpacked_data, 4);
        unpacked_data += 4;
        count+= 4;
    }
    b = read_int_le(unpacked_data);
    unpacked_data += 4;
    count+= 4;
    cliff_tileset_table = new char *[b];
    for (int i = 0; i < b; i++) {
        cliff_tileset_table[i] = new char[4];
        memcpy(cliff_tileset_table[i], unpacked_data, 4);
        unpacked_data += 4;
        count+= 4;
    }
    x = read_int_le(unpacked_data);
    unpacked_data += 4;
    count+= 4;
    y = read_int_le(unpacked_data);
    unpacked_data += 4;
    count+= 4;
    center_offset_x = read_float_le(unpacked_data);
    unpacked_data += 4;
    count+= 4;
    center_offset_y = read_float_le(unpacked_data);
    unpacked_data += 4;
    count+= 4;
    tilesets = std::vector<tileset_entry>(x * y);
    for (int i = 0; i < x * y; i++) {
        tileset_entry entry;
        entry.height = read_int_16_le(unpacked_data);
        unpacked_data += 2;
        count += 2;
        uint16_t data = read_int_16_le(unpacked_data);
        unpacked_data += 2;
        count += 2;
        entry.water_level = 0x6200 & 0x3FFF;
        entry.is_boundary = data & 0xC000;
        unsigned char next = *unpacked_data++;
        count ++;
        unsigned char low = next & 0x0F;
        unsigned char high = next & 0xF0;
        entry.is_camera_boundary = high & 0x0080;
        entry.is_ramp = high & 0x0010;
        entry.is_water = high & 0x0040;
        entry.is_undead = high & 0x0020;
        entry.texture_type = low; // ref to tileset table
        entry.texture_details = *unpacked_data++;
        count ++;
        unsigned char next_ = *unpacked_data++;
        count ++;
        unsigned char low_ = next_ & 0x0F;
        unsigned char high_ = (next_ & 0xF0) >> 4;
        entry.cliff_type = high_;
        entry.tilepoint = low_;
        entry.final_height = (entry.height - 0x2000 + (entry.tilepoint - 2) * 0x0200) / 4;
        tilesets[i] = entry;
    }
    unpacked_data-= count;// for safe deleteing
}

