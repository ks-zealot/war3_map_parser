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
            unsigned local_count = 0;
            l.inclusiveSize = read_int_le(data, local_count);
            l.filterMode = read_int_le(data, local_count);
            l.shadingFlags = read_int_le(data, local_count);
            l.textureId = read_int_le(data, local_count);
            l.textureAnimationId = read_int_le(data, local_count);
            l.coordId = read_int_le(data, local_count);
            l.alpha = read_float_le(data, local_count);
            while (local_count < l.inclusiveSize) {
                unsigned tag = read_int_le(data, local_count);
                switch (tag) {
                    case KMTA: {
                        kmta_handler _kmta_handler(data, local_count);
                        _kmta_handler.parse();
                        l.kmta_tracks = _kmta_handler.get_track_data();
                        break;
                    }

                    case KMTF: {
                        kmtf_handler _kmtf_handler(data, local_count);
                        _kmtf_handler.parse();
                        l.kmtf_tracks = _kmtf_handler.get_track_data();
                        break;
                    }
                }
            }


            count += l.inclusiveSize;
            m.layers.push_back(l);
        }
        count += m.inclusiveSize;
        materials.push_back(m);
    }
}
