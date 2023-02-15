//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_MTLS_HANDLER_H
#define WAR3_MAP_PARSER_MTLS_HANDLER_H


#include "handler.h"
#include "../data/track.h"

struct layer {
    unsigned inclusiveSize;
    unsigned filterMode; // 0: none
    // 1: transparent
    // 2: blend
    // 3: additive
    // 4: add alpha
    // 5: modulate
    // 6: modulate 2x
    unsigned shadingFlags; // 0x1: unshaded
    // 0x2: sphere environment map
    // 0x4: ?
    // 0x8: ?
    // 0x10: two sided
    // 0x20: unfogged
    // 0x40: no depth test
    // 0x80: no depth set
    unsigned textureId;
    unsigned textureAnimationId;
    unsigned coordId;
    float alpha;

//    if (version > 800) {
//        float emissiveGain
//        float[3] fresnelColor
//        float fresnelOpacity
//        float fresnelTeamColor
//    }

    kmtf_track_data kmtf_tracks;
    kmta_track_data kmta_tracks;

//    KMTF: uint32 textureId
//    KMTA: float alpha

//    if (version > 800) {
//        (KMTE)
//    }
//    if (version > 900) {
//        (KFC3)
//                (KFCA)
//                (KFTC)
//    }
};

struct material {
    unsigned inclusiveSize;
    unsigned priorityPlane;
    unsigned flags;

//        if (version > 800) {
//            char[80] shader
//        }

//        char[4] "LAYS";
    unsigned layersCount;
    std::vector<layer> layers;
};

class mtls_handler : public handler {
public:
    mtls_handler(char *data, unsigned size) : handler(data, size) {};

    virtual void parse() override;

private:
    std::vector<material> materials;
};


#endif //WAR3_MAP_PARSER_MTLS_HANDLER_H
