//
// Created by zealot on 05.01.2023.
//

// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "../tinygltf/tiny_gltf.h"
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
    unpacked_data = new unsigned char[sector_size *
                                      offset_table.size()];//todo выделять памяти сколько требуется, а не больше
    for (int i = 0; i < offset_table.size() - 1; i++) {
        read_block(offset_table, i);
    }
    aggregate();
    generate_mesh();
    write_mesh();
}

void enviroment_file_map_parser::read_block(const std::vector<unsigned int> &offset_table, int i) {
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
    add_triangle(last_vertex + 2, last_vertex + 4, 1);

    add_triangle(last_vertex + 1, last_vertex + 2, last_vertex + 3);//bottom face
    add_triangle(last_vertex + 2, last_vertex + 3, last_vertex + 4);
}

void enviroment_file_map_parser::add_triangle(const unsigned &x1, const unsigned &x2, const unsigned &x3) {
    indicies.push_back(x1);
    indicies.push_back(x2);
    indicies.push_back(x3);
}

void enviroment_file_map_parser::write_mesh() {
// Create a model with a single mesh and save it as a gltf file
    tinygltf::Model m;
    tinygltf::Scene scene;
    tinygltf::Mesh mesh;
    tinygltf::Primitive primitive;
    tinygltf::Node node;
    tinygltf::Buffer buffer;
    tinygltf::BufferView bufferView1;
    tinygltf::BufferView bufferView2;
    tinygltf::Accessor accessor1;
    tinygltf::Accessor accessor2;
    tinygltf::Asset asset;

    // This is the raw data buffer.
//    buffer.data = {
//            // 6 bytes of indices and two bytes of padding
//            0x00,0x00, 0x01,0x00, 0x02,0x00, //(0, 1, 2)
//            0x00,0x00,
//            // 36 bytes of floating point numbers
//            0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, // 0, 0, 0
//            0x00,0x00,0x80,0x3f,  0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, // 1, 0, 0
//            0x00,0x00,0x00,0x00,  0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x00 //  0, 1, 0
//    };
//Back face of cube

    buffer.data = {
            // 72 bytes of indices
            0x00, 0x00, 0x02, 0x00, 0x03, 0x00, //(0, 2, 3)
            0x03, 0x00, 0x01, 0x00, 0x00, 0x00, //(3, 1, 0)

            0x00, 0x00, 0x01, 0x00, 0x04, 0x00, //(0, 1, 4)
            0x04, 0x00, 0x01, 0x00, 0x05, 0x00, //(4, 1, 5)

            0x04, 0x00, 0x05, 0x00, 0x07, 0x00, //(4, 5, 7)
            0x07, 0x00, 0x05, 0x00, 0x06, 0x00, //(7, 5, 6)

            0x07, 0x00, 0x06, 0x00, 0x03, 0x00, //(7, 6, 3)
            0x03, 0x00, 0x02, 0x00, 0x07, 0x00, //(3, 2, 7)

            0x01, 0x00, 0x03, 0x00, 0x05, 0x00, //(1, 3, 5)
            0x00, 0x00, 0x01, 0x00, 0x03, 0x00, //(0, 1, 3)

            0x01, 0x00, 0x03, 0x00, 0x05, 0x00, //(1, 3, 5)
            0x00, 0x00, 0x01, 0x00, 0x03, 0x00, //(0, 1, 3)
            // 96 bytes of floating point numbers
// 0x00, 0x00, 0x80, 0xbf
            0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x0f,    0x00, 0x00, 0x00, 0x00, // 0, -1, 0
            0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x0f,    0x00, 0x00, 0x80, 0x3f, // 0, -1, 1
            0x00, 0x00, 0x00, 0x00,    0x00, 0x00, 0x80, 0x3f,    0x00, 0x00, 0x00, 0x00, // 0, 1, 0
            0x00, 0x00, 0x00, 0x00,    0x00, 0x00, 0x80, 0x3f,    0x00, 0x00, 0x80, 0x3f, // 0, 1, 1
            0x00, 0x00, 0x80, 0x3f,   0x00, 0x00, 0x00, 0x0f,    0x00, 0x00, 0x00, 0x00, // 1, -1,0
            0x00, 0x00, 0x80, 0x3f,   0x00, 0x00, 0x00, 0x0f,    0x00, 0x00, 0x80, 0x3f, // 1, -1,1
            0x00, 0x00, 0x80, 0x3f,    0x00, 0x00, 0x80, 0x3f,    0x00, 0x00, 0x80, 0x3f, // 1, 1,1
            0x00, 0x00, 0x80, 0x3f,    0x00, 0x00, 0x80, 0x3f,    0x00, 0x00, 0x00, 0x00, // 1, 1,0
    };

    // "The indices of the vertices (ELEMENT_ARRAY_BUFFER) take up 72 bytes in the
    // start of the buffer.
    bufferView1.buffer = 0;
    bufferView1.byteOffset = 0;
    bufferView1.byteLength = 72;
    bufferView1.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    // The vertices take up 96 bytes (8 vertices * 3 floating points * 4 bytes)
    // at position 40 in the buffer and are of type ARRAY_BUFFER
    bufferView2.buffer = 0;
    bufferView2.byteOffset = 72;
    bufferView2.byteLength = 96;
    bufferView2.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    // Describe the layout of bufferView1, the indices of the vertices
    accessor1.bufferView = 0;
    accessor1.byteOffset = 0;
    accessor1.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
    accessor1.count = 36;
    accessor1.type = TINYGLTF_TYPE_SCALAR;
    accessor1.maxValues.push_back(7);
    accessor1.minValues.push_back(0);

    // Describe the layout of bufferView2, the vertices themself
    accessor2.bufferView = 1;
    accessor2.byteOffset = 0;
    accessor2.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor2.count =8;
    accessor2.type = TINYGLTF_TYPE_VEC3;
    accessor2.maxValues = {1.0, 1.0, 1.0};
    accessor2.minValues = {-1.0, -1.0, -1.0};

    // Build the mesh primitive and add it to the mesh
    primitive.indices = 0;                 // The index of the accessor for the vertex indices
    primitive.attributes["POSITION"] = 1;  // The index of the accessor for positions
    primitive.material = 0;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;
    mesh.primitives.push_back(primitive);

    // Other tie ups
    node.mesh = 0;
    scene.nodes.push_back(0); // Default scene

    // Define the asset. The version is required
    asset.version = "2.0";
    asset.generator = "tinygltf";

    // Now all that remains is to tie back all the loose objects into the
    // our single model.
    m.scenes.push_back(scene);
    m.meshes.push_back(mesh);
    m.nodes.push_back(node);
    m.buffers.push_back(buffer);
    m.bufferViews.push_back(bufferView1);
    m.bufferViews.push_back(bufferView2);
    m.accessors.push_back(accessor1);
    m.accessors.push_back(accessor2);
    m.asset = asset;

    // Create a simple material
    tinygltf::Material mat;
    mat.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.9f, 0.9f, 1.0f};
    mat.doubleSided = true;
    m.materials.push_back(mat);

    // Save it to a file
    tinygltf::TinyGLTF gltf;
    gltf.WriteGltfSceneToFile(&m, "cube.gltf",
                              true, // embedImages
                              true, // embedBuffers
                              true, // pretty print
                              false); // write binary
}

