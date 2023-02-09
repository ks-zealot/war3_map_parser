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
        n.inclusiveSize = read_int_le(data);
        n.name = std::string(data, 80);
        data += 80;
        n.objectId = read_int_le(data);
        n.parentId = read_int_le(data);
        n.flags = read_int_le(data);
        kgtr_handler _kgtr_handler(data);
        _kgtr_handler.parse();
        n.kgtr_tracks = _kgtr_handler.get_tracks();
        kgrt_handler _kgrt_handler(data);
        _kgtr_handler.parse();
        n.kgtr_tracks = _kgtr_handler.get_tracks();
        kgsc_handler _kgsc_handler(data);
        _kgsc_handler.parse();
        n.kgsc_tracks = _kgsc_handler.get_tracks();
        h.node = n;
        count += n.inclusiveSize;
    }
}
