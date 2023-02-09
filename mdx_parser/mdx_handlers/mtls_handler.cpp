//
// Created by zealot on 08.02.2023.
//

#include <cassert>
#include "mtls_handler.h"
#include "../../map_parser/util.h"
#include "track_handlers/kmta_handler.h"
#include "track_handlers/kmtf_handler.h"

void mtls_handler::parse() {
    unsigned count = 0;
    while (count > size) {
        material m;
        m.inclusiveSize = read_int_le(data);
        m.priorityPlane = read_int_le(data);
        m.flags = read_int_le(data);
        std::string LAYS = std::string(data, 4);
        data += 4;
        assert(LAYS == "LAYS");
        unsigned layer_count = read_int_le(data);
        for (int i = 0; i < layer_count; i++) {
            layer l;
            l.inclusiveSize = read_int_le(data);
            l.filterMode = read_int_le(data);
            l.shadingFlags = read_int_le(data);
            l.textureId = read_int_le(data);
            l.textureAnimationId = read_int_le(data);
            l.coordId = read_int_le(data);
            l.alpha = read_float_le(data);
            kmta_handler _kmta_handler(data);
            _kmta_handler.parse();
            l.kmta_tracks = _kmta_handler.get_tracks();
            kmtf_handler _kmtf_handler(data);
            _kmtf_handler.parse();
            l.kmtf_tracks = _kmtf_handler.get_tracks();
            count += l.inclusiveSize;
            m.layers.push_back(l);
        }
        count += m.inclusiveSize;
        materials.push_back(m);
    }
}
