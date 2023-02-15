//
// Created by zealot on 09.02.2023.
//

#include "help_handler.h"
#include "../../map_parser/util.h"
#include "track_handlers/kgtr_handler.h"
#include "track_handlers/kgrt_handler.h"
#include "track_handlers/kgsc_handler.h"

void help_handler::parse() {
    unsigned count = 0;
    while (count < size) {
        helper h;
        node n;
        unsigned local_count = 0;
        n.inclusiveSize = read_int_le(data, local_count);
        n.name = std::string(data, 80);
        data += 80;
        local_count += 80;
        n.objectId = read_int_le(data, local_count);
        n.parentId = read_int_le(data, local_count);
        n.flags = read_int_le(data, local_count);
        if (local_count < n.inclusiveSize) {
            kgtr_handler _kgtr_handler(data, local_count);
            _kgtr_handler.parse();
            n.kgtr_tracks = _kgtr_handler.get_tracks();
        }

        if (local_count < n.inclusiveSize) {
            kgrt_handler _kgrt_handler(data, local_count);
            _kgrt_handler.parse();
            n.kgrt_tracks = _kgrt_handler.get_tracks();
        }

        if (local_count < n.inclusiveSize) {
            kgsc_handler _kgsc_handler(data, local_count);
            _kgsc_handler.parse();
            n.kgsc_tracks = _kgsc_handler.get_tracks();
        }

        kgsc_handler _kgsc_handler(data, local_count);
        _kgsc_handler.parse();
        n.kgsc_tracks = _kgsc_handler.get_tracks();
        h.node = n;
        count += n.inclusiveSize;
    }
}
