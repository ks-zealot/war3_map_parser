//
// Created by zealot on 08.02.2023.
//

#include <cassert>
#include <iostream>
#include "mdx_parser.h"
#include "../map_parser/util.h"
#include "mdx_handlers/vers_handler.h"
#include "mdx_handlers/seqs_handler.h"
#include "mdx_handlers/mtls_handler.h"
#include "mdx_handlers/texs_handler.h"
#include "mdx_handlers/geos_handler.h"
#include "mdx_handlers/geoa_handler.h"
#include "mdx_handlers/bone_handler.h"
#include "mdx_handlers/help_handler.h"
#include "mdx_handlers/pivt_handler.h"
#include "mdx_handlers/modl_handler.h"
#include "mdx_handlers/glbs_handler.h"
#include "../tinygltf/tiny_gltf.h"
#include "../data_objects/parser_inner_object/Vertex.h"

void mdx_parser::parse() {
    char MDLX[4];
    file.read(MDLX, 4);
    std::string _MDLX(MDLX, 4);
    assert(_MDLX == "MDLX");
    while (file.peek() != EOF) {

        unsigned tag_as_int = read_int_le(file);
        unsigned size = read_int_le(file);
        char *data = new char[size];
        file.read(data, size);
        std::string _tag = std::string(static_cast<char *>(static_cast<void *>(&tag_as_int)), 4);
        std::cout << "handle " << _tag << " " << size << std::endl;
        handle_data(tag_as_int, data, size);
        delete[] data;
    }
    produce_model();
}

void mdx_parser::handle_data(unsigned id, char *data, int size) {
    switch (id) {
        case VERS: {
            vers_handler h(data, size);
            h.parse();
            break;
        }
        case SEQS: {
            seqs_handler h(data, size);
            h.parse();
            break;
        }
        case MTLS: {
            mtls_handler h(data, size);
            h.parse();
            break;
        }
        case TEXS: {
            texs_handler h(data, size);
            h.parse();
            break;
        }
        case GEOS: {
            geos_handler h(data, size);
            h.parse();
            model.geosets = h.get_geosets();
            break;
        }
        case GEOA: {
            geoa_handler h(data, size);
            h.parse();
            break;
        }
        case BONE: {
            bone_handler h(data, size);
            h.parse();
            break;
        }
        case HELP: {
            help_handler h(data, size);
            break;
        }
        case MODL: {
            modl_handler h(data, size);
            h.parse();
            break;
        }
        case PIVT: {
            pivt_handler h(data, size);
            h.parse();
            break;
        }
        case GLBS: {
            glbs_handler h(data, size);
            h.parse();
            break;
        }
    }
}

void mdx_parser::produce_model() {
// Create a model with a single mesh and save it as a gltf file



    unsigned count = 0;
    geoset &g = model.geosets[0];
//    for (geoset &g: model.geosets) {
        for (int i = 0; i < g.faceTypeGroupsCount; i++) {
            unsigned face_group_count = g.faceGroups[i];
            unsigned face_group_type = g.faceTypeGroups[i];//triangle
            if (face_group_type != 4) {
                throw std::runtime_error("not supported yet");
            }
            std::vector<Vertex> verticies;
            for (int j = 0; j < g.vertexCount * 3; j += 3) {
                verticies.push_back(Vertex(g.vertexPositions[j], g.vertexPositions[j + 1], g.vertexPositions[j + 2]));
            }
            tinygltf::Scene scene;
            tinygltf::Model m;
            tinygltf::Node node;
            tinygltf::Asset asset;
            tinygltf::Mesh mesh;
            tinygltf::Primitive primitive;
            tinygltf::Buffer buffer;
            tinygltf::BufferView bufferView1;
            tinygltf::BufferView bufferView2;
            tinygltf::Accessor accessor1;
            tinygltf::Accessor accessor2;

            //copy indicies data
            std::vector<unsigned char> indicies_data;
            for (uint16_t &idx: std::vector<uint16_t>(g.faces.begin(), g.faces.begin() + face_group_count)) {
                unsigned char *chr = static_cast<unsigned char *>(static_cast<void *>(&idx));
                char bytes[sizeof idx];
                std::copy(chr,
                          chr + sizeof idx,
                          bytes);
                for (int i = 0; i < sizeof idx; i++) {
                    indicies_data.push_back(bytes[i]);
                }
            }
            //copy vertex data
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
                          bytes + sizeof(float) * 2);
                std::copy(chr3,
                          chr3 + sizeof(float),
                          bytes + (sizeof(float)));
                for (int i = 0; i < sizeof(float) * 3; i++) {
                    verticies_data.push_back(bytes[i]);
                }
            }
            buffer.data.insert(buffer.data.end(), indicies_data.begin(), indicies_data.end());
            buffer.data.insert(buffer.data.end(), verticies_data.begin(), verticies_data.end());

            bufferView1.buffer = count;
            bufferView1.byteOffset = 0;
            bufferView1.byteLength = indicies_data.size();
            bufferView1.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

            bufferView2.buffer = count;
            bufferView2.byteOffset = indicies_data.size();
            bufferView2.byteLength = verticies_data.size();
            bufferView2.target = TINYGLTF_TARGET_ARRAY_BUFFER;

            // Describe the layout of bufferView1, the indices of the vertices
            accessor1.bufferView = count * 2;
            accessor1.byteOffset = 0;
            accessor1.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
            accessor1.count = face_group_count;
            accessor1.type = TINYGLTF_TYPE_SCALAR;

            // Describe the layout of bufferView2, the vertices themself
            accessor2.bufferView = count * 2 + 1;
            accessor2.byteOffset = 0;
            accessor2.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
            accessor2.count = verticies.size();//8;
            accessor2.type = TINYGLTF_TYPE_VEC3;
            // Build the mesh primitive and add it to the mesh
            primitive.indices = count * 2;                 // The index of the accessor for the vertex indices
            primitive.attributes["POSITION"] =count * 2 + 1;  // The index of the accessor for positions
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
            asset.version = "2.0";
            asset.generator = "tinygltf";
            m.asset = asset;
            tinygltf::Material mat;
            mat.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.9f, 0.9f, 1.0f};
            mat.doubleSided = true;
            m.materials.push_back(mat);
            tinygltf::TinyGLTF gltf;
            gltf.WriteGltfSceneToFile(&m, "tree.gltf", //todo remove hardcode
                                      true, // embedImages
                                      true, // embedBuffers
                                      true, // pretty print
                                      false); // write binary
        }
//    }

    // Other tie ups


    // Define the asset. The version is required



}


 