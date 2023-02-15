//
// Created by zealot on 09.02.2023.
//

#include "kgao_handler.h"
#include "../../../map_parser/util.h"

int kgao_handler::get_tag() {
    return KGAO;
}

void kgao_handler::parse_track() {
    kgao_track truck;
    truck.frame = read_int_le(data, count);
    truck.value = read_float_le(data, count);
    if (interpolation_type > 0) {
        truck.inTan = read_float_le(data, count);
        truck.outTan = read_float_le(data, count);
    }
    track_data.tracks.push_back(truck);
}

void kgao_handler::parse() {
    track_handler::parse();
    track_data.interpolationType = interpolation_type;
    track_data.globalSequenceId = global_sequece_id;
}
