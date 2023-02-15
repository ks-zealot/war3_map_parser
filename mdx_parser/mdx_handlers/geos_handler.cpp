//
// Created by zealot on 09.02.2023.
//

#include <cassert>
#include "geos_handler.h"
#include "../../map_parser/util.h"

void geos_handler::parse() {
    unsigned count = 0;
    while (count < size) {
        geoset g;
        g.inclusiveSize = read_int_le(data);
        std::string VRTX = std::string(data, 4);
        data += 4;
        assert(VRTX == "VRTX");
        g.vertexCount = read_int_le(data);
        g.vertexPositions.reserve(g.vertexCount * 3);
        for (int i = 0; i < g.vertexCount * 3; i++) {
            g.vertexPositions.push_back(read_float_le(data));
        }
        std::string NRMS = std::string(data, 4);
        assert(NRMS == "NRMS");
        data += 4;
        g.normalCount = read_int_le(data);
        g.vertexNormals.reserve(g.normalCount * 3);
        for (int i = 0; i < g.normalCount * 3; i++) {
            g.vertexNormals.push_back(read_float_le(data));
        }
        std::string PTYP = std::string(data, 4);
        assert(PTYP == "PTYP");
        data += 4;
        g.faceTypeGroupsCount = read_int_le(data);
        g.faceTypeGroups.reserve(g.faceTypeGroupsCount);
        for (int i = 0; i < g.faceTypeGroupsCount; i++) {
            g.faceTypeGroups.push_back(read_int_le(data));
        }
        std::string PCNT = std::string(data, 4);
        assert(PCNT == "PCNT");
        data += 4;
        g.faceGroupsCount = read_int_le(data);
        g.faceGroups.reserve(g.faceGroupsCount);
        for (int i = 0; i < g.faceGroupsCount; i++) {
            g.faceGroups.push_back(read_int_le(data));
        }
        std::string PVTX = std::string(data, 4);
        assert(PVTX == "PVTX");
        data += 4;
        g.facesCount = read_int_le(data);
        g.faces.reserve(g.facesCount);
        for (int i = 0; i < g.facesCount; i++) {
            g.faces.push_back(read_int_16_le(data));
        }
        std::string GNDX = std::string(data, 4);
        assert(GNDX == "GNDX");
        data += 4;
        g.vertexGroupsCount = read_int_le(data);
        for (int i = 0; i < g.vertexGroupsCount; i++) {
            g.vertexGroups.push_back(*data++);
        }
        std::string MTGC = std::string(data, 4);
        assert(MTGC == "MTGC");
        data += 4;
        g.matrixGroupsCount = read_int_le(data);
        g.matrixGroups.reserve(g.matrixGroupsCount);
        for (int i = 0; i < g.matrixGroupsCount; i++) {
            g.matrixGroups.push_back(read_int_le(data));
        }
        std::string MATS = std::string(data, 4);
        assert(MATS == "MATS");
        data += 4;
        g.matrixIndicesCount = read_int_le(data);
        g.matrixIndices.reserve(g.matrixIndicesCount);
        for (int i = 0; i < g.matrixIndicesCount; i++) {
            g.matrixIndices.push_back(read_int_le(data));
        }
        g.materialId = read_int_le(data);
        g.selectionGroup = read_int_le(data);
        g.selectionFlags = read_int_le(data);
        g._extent = calculate_extent(data);
        g.extentsCount = read_int_le(data);
        for (int i = 0; i < g.extentsCount; i++) {
            g.sequenceExtents.push_back(calculate_extent(data));
        }
        std::string UVAS = std::string(data, 4);
        assert(UVAS == "UVAS");
        data += 4;
        g.textureCoordinateSetsCount = read_int_le(data);
        g.textureCoordinateSets.reserve(g.textureCoordinateSetsCount);
        for (int i = 0; i < g.textureCoordinateSetsCount; i++) {
            g.textureCoordinateSets.push_back(calculate_texture_set(data));
        }
        geosets.push_back(g);
        count += g.inclusiveSize;
    }
}

extent geos_handler::calculate_extent(char *&data) {
    extent e;
    e.boundsRadius = read_float_le(data);

    e.minimum.push_back(read_float_le(data));
    e.minimum.push_back(read_float_le(data));
    e.minimum.push_back(read_float_le(data));

    e.maximum.push_back(read_float_le(data));
    e.maximum.push_back(read_float_le(data));
    e.maximum.push_back(read_float_le(data));
    return e;
}

texture_coordinate_set geos_handler::calculate_texture_set(char *&data) {
    std::string UVBS = std::string(data, 4);
    assert(UVBS == "UVBS");
    data += 4;
    texture_coordinate_set ts;
    ts.count = read_int_le(data);
    ts.texture_coordinates.reserve(ts.count * 2);
    for (int i = 0; i < ts.count * 2; i++) {
        ts.texture_coordinates.push_back(read_float_le(data));
    }
    return ts;
}
