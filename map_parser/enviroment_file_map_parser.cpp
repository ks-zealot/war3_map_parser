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
    unpacked_data = new unsigned char[sector_size * offset_table.size()];
    for (int i = 0; i < offset_table.size() - 1; i++) {
        read_block(offset_table, i);
    }
    aggregate();
    generate_mesh();
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
    count += 4;
    assert (w3e == "W3E!");
    unsigned version = read_int_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    assert(version == 11);
    main_tileset = *unpacked_data++;
    count++;
    custom_tilesets_flag = read_int_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    a = read_int_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    tileset_table = new char *[a];
    for (int i = 0; i < a; i++) {
        tileset_table[i] = new char[4];
        memcpy(tileset_table[i], unpacked_data, 4);
        unpacked_data += 4;
        count += 4;
    }
    b = read_int_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    cliff_tileset_table = new char *[b];
    for (int i = 0; i < b; i++) {
        cliff_tileset_table[i] = new char[4];
        memcpy(cliff_tileset_table[i], unpacked_data, 4);
        unpacked_data += 4;
        count += 4;
    }
    x = read_int_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    y = read_int_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    center_offset_x = read_float_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    center_offset_y = read_float_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    tilesets = std::vector<tileset_entry>(x * y);
    for (int i = 0; i < x * y; i++) {
        tileset_entry entry;
        entry.height = read_sint_16_le(unpacked_data);
        unpacked_data += 2;
        count += 2;
        int16_t data = read_int_16_le(unpacked_data);
        unpacked_data += 2;
        count += 2;
        entry.water_level = 0x6200 & 0x3FFF;
        entry.is_boundary = data & 0xC000;
        unsigned char next = *unpacked_data++;
        count++;
        unsigned char low = next & 0x0F;
        unsigned char high = next & 0xF0;
        entry.is_camera_boundary = high & 0x0080;
        entry.is_ramp = high & 0x0010;
        entry.is_water = high & 0x0040;
        entry.is_undead = high & 0x0020;
        entry.texture_type = low; // ref to tileset table
        entry.texture_details = *unpacked_data++;
        count++;
        unsigned char next_ = *unpacked_data++;
        count++;
        unsigned char low_ = next_ & 0x0F;
        unsigned char high_ = (next_ & 0xF0) >> 4;
        entry.cliff_type = high_;
        entry.tilepoint = low_;
        entry.final_height = (entry.height - 0x2000 + (entry.tilepoint - 2) * 0x0200) / 4;
        tilesets[i] = entry;
    }

    unpacked_data -= count;// for safe deleteing
}

/*
 * собсна, в даноой ситуации нам надо
 * создать вектор вершин, который будет состоять из структур хранящих внутри себя три шорт инт
 * Создать вектор граней, который будет хранить в себе unsigned
 * сгенерить процедурный точки с координатами по углам будущей карты, и z 0.
 *
 */


void enviroment_file_map_parser::generate_mesh() {
    unsigned minX = 0;
    unsigned minY = 0;
    unsigned maxX = x;
    unsigned maxY = y;
    unsigned tile_size = 400;
    unsigned count = 0;

    /**
     * нам нужны всего x * y точек плюс 4
     * запихиваем первую точку lb_corner (0 , 0)
     * потом первую чтрочку
     * потом rb_corner (x, 0) x + 1
     * потом вск кроме последней точки
     * потом lt_corner (0, y ) (x * y -y ) + 2
     * потом все что осталось
     * потом rt_corner (x, y ) (x * Y ) + 3
     * собсна потом нам остается топлько пострить индексы  для каждой дочки x,y запихать (x, y; x+ 1, y; x, y + 1) и
     * (x, y; x- 1, y; x, y - 1) если такоые сущестуют
     * сложнее всего будет построить фундамент
     *  тут индексы
     *  0, 1 , x для первого угла
     *  x + 1 , 0 , 1 для второго, это построит переднуюю грань
     *
     *  (x * y -y ) + 2   ,(x * y -y ) + 3 , (x * Y ) + 3
     * (x * Y ) + 3,  (x * y  ) + 2 ,(x * y -y ) + 3  задняя грань
     *
     * 0 , (x * y -y ) + 2, (x * y -y ) + 3
     * (x * y -y ) + 3, 1, 0 // левая грань
     *
     *
     * x + 1 ,(x * Y ) + 3 , (x * Y ) + 2
     * (x * Y ) + 3, (x * Y ) + 2, x// правая грань
     */
    for (int i = 0; i < x; i++) {
        for (int k = 0; k < y; k++) {
            tileset_entry &ts = tilesets[k + (i * x)];

            verticies.push_back(Vertex(i * tile_size, k * tile_size, ts.height));

            if (i > minX && k > minY) {
                add_triangle(count, count - 1, count - x - 1);//треугольник направленный налево вниз

            }
            if (i <= maxX && k <= maxY) {
                add_triangle(count, count + 1, count + x + 1);//треуголник направленный направо вверх
            }
            count++;
        }
    }
    verticies.push_back(Vertex(0, 0, 0)); //lb x * y + 1
    verticies.push_back(Vertex(0, y * tile_size, 0));//rb x * y + 2
    verticies.push_back(Vertex(x * tile_size, 0, 0));//lt x * y + 3
    verticies.push_back(Vertex(x * tile_size, y * tile_size, 0));// rt x * y + 4
    unsigned last_vertex = x * y;
    add_triangle(last_vertex + 1, 0, 1);
    add_triangle(last_vertex + 1, last_vertex + 2, 1);//front face


    add_triangle(last_vertex + 3, last_vertex - x, last_vertex);//back face
    add_triangle(last_vertex + 3, last_vertex + 4, last_vertex);

    add_triangle(last_vertex + 1, 0, last_vertex + 3);//left face
    add_triangle(0, last_vertex - x, last_vertex + 3);

    add_triangle(last_vertex + 2, 1, last_vertex);//right face
    add_triangle(last_vertex + 2,last_vertex + 4, 1);

    add_triangle(last_vertex + 1, last_vertex + 2, last_vertex + 3);//bottom face
    add_triangle(last_vertex + 2, last_vertex + 3,last_vertex + 4);
}

void enviroment_file_map_parser::add_triangle(const unsigned &x1, const unsigned &x2, const unsigned &x3) {
    indicies.push_back(x1);
    indicies.push_back(x2);
    indicies.push_back(x3);
}

