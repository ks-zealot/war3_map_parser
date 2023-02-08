//
// Created by zealot on 08.02.2023.
//

#include <cassert>
#include "mtls_handler.h"
#include "../../map_parser/util.h"

void mtls_handler::parse() {
    unsigned count = 0;
    while (count > size) {
        material m;
        m.inclusiveSize = read_int_le(data);
        data += 4;
        m.priorityPlane = read_int_le(data);
        data += 4;
        m.flags = read_int_le(data);
        std::string LAYS = std::string(data, 4);
        data += 4;
        assert(LAYS == "LAYS");
        unsigned layer_count = read_int_le(data);
        data += 4;
        for (int i = 0; i < layer_count; i++) {
            layer l;
            l.inclusiveSize = read_int_le(data);
            data += 4;
            l.filterMode = read_int_le(data);
            data += 4;
            l.shadingFlags = read_int_le(data);
            data += 4;
            l.textureId = read_int_le(data);
            data += 4;
            l.textureAnimationId = read_int_le(data);
            data += 4;
            l.coordId = read_int_le(data);
            data += 4;
            l.alpha = read_float_le(data);
            data += 4;

            count += l.inclusiveSize;
            m.layers.push_back(l);
        }
        count += m.inclusiveSize;
        materials.push_back(m);
    }
}
