//
// Created by zealot on 09.02.2023.
//
#include <string>
#include "../mdx_handlers/track_handlers/kgrt_handler.h"
#include "../mdx_handlers/track_handlers/kgtr_handler.h"
#include "../mdx_handlers/track_handlers/kgsc_handler.h"

#ifndef WAR3_MAP_PARSER_NODE_H
#define WAR3_MAP_PARSER_NODE_H

struct node {
    unsigned inclusiveSize;
    std::string name;
    unsigned objectId;
    unsigned parentId;
    unsigned flags; // 0x0: helper
    // 0x1: dont inherit translation
    // 0x2: dont inherit rotation
    // 0x4: dont inherit scaling
    // 0x8: billboarded
    // 0x10: billboarded lock x
    // 0x20: billboarded lock y
    // 0x40: billboarded lock z
    // 0x80: camera anchored
    // 0x100: bone
    // 0x200: light
    // 0x400 event object
    // 0x800: attachment
    // 0x1000 particle emitter
    // 0x2000: collision shape
    // 0x4000: ribbon emitter
    // 0x8000: if particle emitter: emitter uses mdl, if particle emitter 2: unshaded
    // 0x10000: if particle emitter: emitter uses tga, if particle emitter 2: sort primitives far z
    // 0x20000: line emitter
    // 0x40000: unfogged
    // 0x80000: model space
    // 0x100000: xy quad
    kgtr_track_data kgtr_track_data;
    kgrt_track_data kgrt_track_data;
    kgsc_track_data kgsc_track_data;
//    (KGTR)
//            (KGRT)
//            (KGSC)
};
#endif //WAR3_MAP_PARSER_NODE_H
