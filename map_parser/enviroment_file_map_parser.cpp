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
    parser.parse();
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
        std::string id(tileset_table[i], 4);
        tileset_textures[id] = png::image<png::rgb_pixel>(
                "/home/zealot/CLionProjects/War3_Map_Parser/assets/" + parser.texture_name(id));
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
    result_texture = png::image<png::rgb_pixel>(x * 64, y * 64);
    count += 4;
    center_offset_x = read_float_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    center_offset_y = read_float_le(unpacked_data);
    unpacked_data += 4;
    count += 4;
    tilesets = std::vector<tileset_entry>(x * y);

    png::image<png::rgb_pixel> texture(x * 64, y * 64);
    for (int i = 0; i < x * y; i++) {
        tileset_entry entry;
        entry.height = read_sint_16_le(unpacked_data);
        unpacked_data += 2;
        count += 2;
        int16_t data = read_int_16_le(unpacked_data);
        unpacked_data += 2;
        count += 2;
        entry.water_level = 0x6200 & 0x3FFF;//todo ??
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
        std::string s(tileset_table[low], 4);
        if (low > a) {
            throw std::runtime_error("invalid texture");
        }

//        std::cout << "texture name " << parser.texture_name(s) <<std::endl;
        entry.texture_details = *unpacked_data++;
        entry.texture_details = entry.texture_details & 0x1f;
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
    unsigned tile_size = 1;//400;
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


    for (int k = 0; k < y; k++) {
        for (int i = 0; i < x; i++) {
            tileset_entry &ts = tilesets[i + (k * x)];
            std::string s(tileset_table[ts.texture_type], 4);
            png::image<png::rgb_pixel> &current_texture = tileset_textures[s];
            int pos = ts.texture_details;
            int y_shift = pos / 4;
            int x_shift = pos % 4;
            int idx = 0;
            if ( i ==0 && k == 0) {
                texcoords.push_back(TexCoord(0.0001, 0.0001));
            }
            if ( i ==x && k == 0) {
                texcoords.push_back(TexCoord(0.9999, 0.0001));
            }
            if ( i ==0 && k == y) {
                texcoords.push_back(TexCoord(0.0001, 0.9999));
            }
            if ( i ==x&& k == y) {
                texcoords.push_back(TexCoord(0.9999, 0.9999));
            } else {
                float u = i * tile_size / (x * tile_size);
                float w = k * tile_size / (y * tile_size);
                texcoords.push_back(TexCoord(u, w));
            }

            if (pos < 16) {
                for (int _x = 256 + x_shift * 64, __x = 0; _x < (256 + x_shift * 64) + 64; ++_x, ++__x) {
                    for (int _y = y_shift * 64, __y = 0; _y < (y_shift * 64) + 64; ++_y, ++__y) {
                        int global_x = __x + (i * 64);
                        int global_y = __y + (k * 64);
                        png::rgb_pixel pxl = current_texture.get_pixel(_x, _y);
                        result_texture.set_pixel(global_x, global_y, pxl);
                    }
                }
            } else if (pos == 17) {
                for (int _x = 192, __x = 0; _x < 256; ++_x, ++__x) {
                    for (int _y = 192, __y = 0; _y < 256; ++_y, ++__y) {
                        int global_x = __x + (i * 64);
                        int global_y = __y + (k * 64);
                        png::rgb_pixel pxl = current_texture.get_pixel(_x, _y);
                        result_texture.set_pixel(global_x, global_y, pxl);
                    }
                }
            } else {
                for (int _x = 0, __x = 0; _x < 64; ++_x, ++__x) {
                    for (int _y = 0, __y = 0; _y < 64; ++_y, ++__y) {
                        int global_x = __x + (i * 64);
                        int global_y = __y + (k * 64);
                        png::rgb_pixel pxl = current_texture.get_pixel(_x, _y);
                        result_texture.set_pixel(global_x, global_y, pxl);
                    }
                }
            }
        }
    }
//    result_texture.write("output.png");
    for (int k = 0; k < y; k++) {
        for (int i = 0; i < x; i++) {
            tileset_entry &ts = tilesets[i + (k * x)];

            verticies.push_back(Vertex(i * tile_size, k * tile_size, ts.height / 1000.f));

            if (i > minX && k > minY) {
                add_triangle(count, count - 1, count - x);//треугольник направленный налево вниз

            }
            if (i < maxX - 1 && k < maxY - 1) {
                add_triangle(count, count + 1, count + x);//треуголник направленный направо вверх
            }
            count++;
        }
    }
//    assert(verticies.size() == x * y);
    verticies.push_back(Vertex(0, 0, 0)); //lb x * y + 1
    verticies.push_back(Vertex(0, y * tile_size, 0));//rb x * y + 2
    verticies.push_back(Vertex(x * tile_size, 0, 0));//lt x * y + 3
    verticies.push_back(Vertex(x * tile_size, y * tile_size, 0));// rt x * y + 4
    texcoords.push_back(TexCoord(0, 0));
    texcoords.push_back(TexCoord(0, 1));
    texcoords.push_back(TexCoord(1, 0));
    texcoords.push_back(TexCoord(1, 1));
    unsigned last_vertex = x * y  ;
//    assert(verticies.size() == x * y + 4);
    add_triangle(last_vertex + 1, 0, y);
    add_triangle(last_vertex, last_vertex + 1, y);//front face

    add_triangle(last_vertex + 2, x * y - y, x * y);//back face
    add_triangle(last_vertex + 2, last_vertex + 3, x * y);

    add_triangle(last_vertex, 0, x * y - y);//left face
    add_triangle(last_vertex, last_vertex + 2, x * y - y);
//
    add_triangle(last_vertex + 1, y, x * y);//right face
    add_triangle(last_vertex + 1, last_vertex + 3, x * y);
//
    add_triangle(last_vertex, last_vertex + 1, last_vertex + 3);//bottom face
    add_triangle(last_vertex, last_vertex + 2, last_vertex + 3);
}

void enviroment_file_map_parser::add_triangle(const unsigned &x1, const unsigned &x2, const unsigned &x3) {
    indicies.push_back(x1);
    indicies.push_back(x2);
    indicies.push_back(x3);
    if (x1 > x * y + 4 || x2 > x * y + 4 || x3 > x * y + 4) {
        throw std::runtime_error("invalid index");
    }
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
    tinygltf::BufferView bufferView3;
//    tinygltf::BufferView bufferView4;
    tinygltf::Accessor accessor1;
    tinygltf::Accessor accessor2;
    tinygltf::Accessor accessor3;
    tinygltf::Asset asset;

    std::vector<unsigned char> indicies_data;
    for (unsigned &idx: indicies) {
        if (idx > x * y + 4) {
            throw std::runtime_error("invalid index");
        }
        if (idx > verticies.size()) {
            throw std::runtime_error("invalid index");
        }
        unsigned char *chr = static_cast<unsigned char *>(static_cast<void *>(&idx));
        char bytes[sizeof idx];
        std::copy(chr,
                  chr + sizeof idx,
                  bytes);
        for (int i = 0; i < sizeof idx; i++) {
            indicies_data.push_back(bytes[i]);
        }
    }
    std::vector<unsigned char> verticies_data;
    for (Vertex &v: verticies) {
        char bytes[sizeof(float) * 3];

        unsigned char *chr1 = static_cast<unsigned char *>(static_cast<void *>(&v.x));
        unsigned char *chr2 = static_cast<unsigned char *>(static_cast<void *>(&v.y));
        unsigned char *chr3 = static_cast<unsigned char *>(static_cast<void *>(&v.z));
        std::copy(chr1,
                  chr1 + sizeof(float),
                  bytes);
        std::copy(chr2,
                  chr2 + sizeof(float),
                  bytes + sizeof(float));
        std::copy(chr3,
                  chr3 + sizeof(float),
                  bytes + (sizeof(float)) * 2);
        for (int i = 0; i < sizeof(float) * 3; i++) {
            verticies_data.push_back(bytes[i]);
        }
    }
    std::vector<unsigned char> texcoord_data;

    buffer.data.insert(buffer.data.end(), indicies_data.begin(), indicies_data.end());
    buffer.data.insert(buffer.data.end(), verticies_data.begin(), verticies_data.end());
    for (TexCoord &t: texcoords) {
        char bytes[sizeof(float) * 2];
        unsigned char *chr1 = static_cast<unsigned char *>(static_cast<void *>(&t.u));
        unsigned char *chr2 = static_cast<unsigned char *>(static_cast<void *>(&t.w));
        std::copy(chr1,
                  chr1 + sizeof(float),
                  bytes);
        std::copy(chr2,
                  chr2 + sizeof(float),
                  bytes + sizeof(float));
        for (int i = 0; i < sizeof(float) * 2; i++) {
            texcoord_data.push_back(bytes[i]);
        }
    }
    buffer.data.insert(buffer.data.end(), texcoord_data.begin(), texcoord_data.end());
    // "The indices of the vertices (ELEMENT_ARRAY_BUFFER) take up 72 bytes in the
    // start of the buffer.
    bufferView1.buffer = 0;
    bufferView1.byteOffset = 0;
    bufferView1.byteLength = indicies_data.size();//72;
    bufferView1.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    // The vertices take up 96 bytes (8 vertices * 3 floating points * 4 bytes)
    // at position 40 in the buffer and are of type ARRAY_BUFFER
    bufferView2.buffer = 0;
    bufferView2.byteOffset = indicies_data.size();
    bufferView2.byteLength = verticies_data.size();
    bufferView2.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    bufferView3.buffer = 0;
    bufferView3.byteOffset = indicies_data.size() + verticies_data.size();
    bufferView3.byteLength = texcoord_data.size();
    bufferView3.target = TINYGLTF_TARGET_ARRAY_BUFFER;



    // Describe the layout of bufferView1, the indices of the vertices
    accessor1.bufferView = 0;
    accessor1.byteOffset = 0;
    accessor1.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;// TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
    accessor1.count = indicies.size();//36;
    accessor1.type = TINYGLTF_TYPE_SCALAR;
//    accessor1.maxValues.push_back(7);
//    accessor1.minValues.push_back(0);

    // Describe the layout of bufferView2, the vertices themself
    accessor2.bufferView = 1;
    accessor2.byteOffset = 0;
    accessor2.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor2.count = verticies.size();//8;
    accessor2.type = TINYGLTF_TYPE_VEC3;

    accessor3.bufferView = 2;
    accessor3.byteOffset = 0;
    accessor3.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor3.count = texcoords.size();//8;
    accessor3.type = TINYGLTF_TYPE_VEC2;
    accessor2.maxValues = {1.0, 1.0, 1.0};// todo чтобы вернуть, надо переопределить операторы больше меньше у вершины
    accessor2.minValues = {-1.0, -1.0, -1.0};
    accessor3.maxValues = {1.0, 1.0};
    accessor3.minValues = {-1.0, -1.0};
    // Build the mesh primitive and add it to the mesh
    primitive.indices = 0;                 // The index of the accessor for the vertex indices
    primitive.attributes["POSITION"] = 1;  // The index of the accessor for positions
    primitive.attributes["TEXCOORD_0"] = 2;  // The index of the accessor for positions
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
    m.bufferViews.push_back(bufferView3);
    m.accessors.push_back(accessor1);
    m.accessors.push_back(accessor2);
    m.accessors.push_back(accessor3);
    m.asset = asset;
    tinygltf::TextureInfo tx;
    tx.index = 0;
    // Create a simple material
    tinygltf::Material mat;
    mat.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.9f, 0.9f, 1.0f};
    mat.pbrMetallicRoughness.baseColorTexture = tx;
    mat.doubleSided = true;
    m.materials.push_back(mat);
    tinygltf::Image img;
    img.width = x * 64;
    img.height = y * 64;
    img.mimeType = "image/png";
    img.uri = "/home/zealot/CLionProjects/War3_Map_Parser/output.png";
//    tinygltf::Sampler sample;
//    sample.magFilter = 9729;
//    sample.minFilter = 9987;
//    sample.wrapS = TINYGLTF_TEXTURE_WRAP_REPEAT;
//    sample.wrapT = 33648;
//    img.bufferView = 3;
//    std::stringstream stream;
//    result_texture.write_stream(stream);
//    std::string s = stream.str();
//    buffer.data.insert(buffer.data.end(), s.begin(), s.end());
//    m.buffers.push_back(buffer);
//    bufferView4.buffer = 0;
//    bufferView4.byteOffset = indicies_data.size() + verticies_data.size() + texcoords.size();
//    bufferView4.byteLength = s.size();
//    bufferView4.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
//    m.bufferViews.push_back(bufferView4);
    m.images.push_back(img);
    tinygltf::Texture tex;
    tex.source = 0;
//    tex.sampler = 0;
    m.textures.push_back(tex);
//    m.samplers.push_back(sample);
    // Save it to a file
    tinygltf::TinyGLTF gltf;
    gltf.WriteGltfSceneToFile(&m, "map.gltf", //todo remove hardcode
                              true, // embedImages
                              true, // embedBuffers
                              true, // pretty print
                              false); // write binary
}

