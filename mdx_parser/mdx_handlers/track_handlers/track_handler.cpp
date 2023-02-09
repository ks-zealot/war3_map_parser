//
// Created by zealot on 08.02.2023.
//

#include <cassert>
#include "track_handler.h"
#include "../../../map_parser/util.h"

void track_handler::parse() {
    tag = read_int_le(data);
    assert(tag = get_tag());
    tracks_count  = read_int_le(data);
    interpolation_type = read_int_le(data);
    global_sequece_id = read_int_le(data);
    for (int i =0; i < tracks_count; i++) {
        parse_track();
    }
}
