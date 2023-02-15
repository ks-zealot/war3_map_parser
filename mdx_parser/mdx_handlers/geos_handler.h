//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_GEOS_HANDLER_H
#define WAR3_MAP_PARSER_GEOS_HANDLER_H


#include <cstdint>
#include "handler.h"
#include "../data/extent.h"

struct texture_coordinate_set {
    unsigned count;
    std::vector<float> texture_coordinates;
};

struct geoset {
    unsigned inclusiveSize;
    unsigned vertexCount;
    std::vector<float> vertexPositions;
    unsigned normalCount;
    std::vector<float> vertexNormals;
    unsigned faceTypeGroupsCount;
    std::vector<unsigned> faceTypeGroups;// 0: points
    // 1: lines
    // 2: line loop
    // 3: line strip
    // 4: triangles
    // 5: triangle strip
    // 6: triangle fan
    // 7: quads
    // 8: quad strip
    // 9: polygons
    unsigned faceGroupsCount;
    std::vector<unsigned> faceGroups;
    unsigned facesCount;
    std::vector<uint16_t> faces;
    unsigned vertexGroupsCount;
    std::vector<unsigned char> vertexGroups;
    unsigned matrixGroupsCount;
    std::vector<unsigned> matrixGroups;
    unsigned matrixIndicesCount;
    std::vector<unsigned> matrixIndices;
    unsigned materialId;
    unsigned selectionGroup;
    unsigned selectionFlags;

//    if (version > 800) {
//        uint32 lod
//        char[80] lodName
//    }

    extent _extent;
    unsigned extentsCount;
    std::vector<extent> sequenceExtents;

//    if (version > 800) {
//        (Tangents)
//                (Skin)
//    }Skin

    unsigned textureCoordinateSetsCount;
    std::vector<texture_coordinate_set> textureCoordinateSets;
};

class geos_handler : public handler {
public:
    geos_handler(char *data, unsigned size) : handler(data, size) {};

    virtual void parse() override;

private:
    std::vector<geoset> geosets;

    extent calculate_extent(char *&data);

    texture_coordinate_set calculate_texture_set(char *&data);
};


#endif //WAR3_MAP_PARSER_GEOS_HANDLER_H
