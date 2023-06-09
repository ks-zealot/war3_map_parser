//
// Created by zealot on 09.02.2023.
//

#include "bone_handler.h"
#include "../../map_parser/util.h"
#include "track_handlers/kgtr_handler.h"
#include "track_handlers/kgrt_handler.h"
#include "track_handlers/kgsc_handler.h"

void bone_handler::parse() {
    unsigned count = 0;
    while (count < size) {
        bone b;
        node n;
        unsigned local_count = 0;
        n.inclusiveSize = read_int_le(data, local_count);
        n.name = std::string(data, 80);
        data += 80;
        local_count += 80;
        n.objectId = read_int_le(data, local_count);
        n.parentId = read_int_le(data, local_count);
        n.flags = read_int_le(data, local_count);

        while (local_count < n.inclusiveSize) {
            unsigned tag = read_int_le(data, local_count);
            switch (tag) {
                case KGTR: {
                    kgtr_handler _kgtr_handler(data, local_count);
                    _kgtr_handler.parse();
                    n.kgtr_track_data = _kgtr_handler.get_track_data();
                    break;
                }

                case KGRT: {
                    kgrt_handler _kgrt_handler(data, local_count);
                    _kgrt_handler.parse();
                    n.kgrt_track_data = _kgrt_handler.get_track_data();
                    break;
                }

                case KGSC: {
                    kgsc_handler _kgsc_handler(data, local_count);
                    _kgsc_handler.parse();
                    n.kgsc_track_data = _kgsc_handler.get_track_data();
                    break;
                }

            }
        }

        b.node = n;
        b.geosetId = read_int_le(data);
        b.geosetAnimationId = read_int_le(data);
        count += 4;
        count += 4;
        count += n.inclusiveSize;
    }

}
