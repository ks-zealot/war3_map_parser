//
// Created by zealot on 09.02.2023.
//

#include "kmta_handler.h"
#include "../../../map_parser/util.h"

int kmta_handler::get_tag() {
    return KMTA;
}

void kmta_handler::parse_track() {
    kmta_track track;
    track.frame = read_int_le(data, count);
    track.value = read_float_le(data, count);
    if (interpolation_type > 0) {
        track.inTan = read_float_le(data, count);
        track.outTan = read_float_le(data, count);
    }
    track_data.tracks.push_back(track);
}

void kmta_handler::parse() {
    track_handler::parse();
    track_data.interpolationType = interpolation_type;
    track_data.globalSequenceId = global_sequece_id;
}
