//
// Created by zealot on 05.01.2023.
//

#include <vector>
#include <cstring>
#include <iostream>
#include "abstract_map_file_parser.h"
#include "../decompressors/abstract_decompressor.h"

abstract_map_file_parser::~abstract_map_file_parser() {
    delete[] unpacked_data;
}

void abstract_map_file_parser::read_block(const std::vector<unsigned int> &offset_table, int i) {
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

void abstract_map_file_parser::parse() {
    unsigned bytes_to_read = bp.is_file_compress ? bp.file_comp_size : bp.file_size;
    char *data = new char[bytes_to_read];
    _map.seekg(512 + bp.file_pos); //todo remove hardcode
    _map.read(data, bytes_to_read);
    if (bp.is_file_encrypted) {
        std::cout << "file  encrypted" << std::endl;
    }
    if (bp.is_file_compress) {
        std::cout << " file    compressed" << std::endl;
    }
    unsigned *casted = (unsigned *) (data);
    std::vector<unsigned> offset_table;
    unsigned _offset = *casted++;
    while (_offset != bp.file_comp_size) {
        offset_table.push_back(_offset);
        _offset = *casted++;
    }
    offset_table.push_back(_offset);//todo убрать хак
    unsigned sector_size = 4096;//todo remove hardcode
    unpacked_data = new char[sector_size *
                             offset_table.size()];//todo выделять памяти сколько требуется, а не больше
    for (int i = 0; i < offset_table.size() - 1; i++) {
        read_block(offset_table, i);
    }
    delete[] data;
    aggregate();
}
