//
// Created by zealot on 09.02.2023.
//

#include "kgsc_handler.h"
#include "../../../map_parser/util.h"

int kgsc_handler::get_tag() {
    return KGSC;
}

void kgsc_handler::parse_track() {
    kgsc_track truck;
    truck.frame = read_int_le(data, count);
    truck.value = scaling(read_float_le(data, count), read_float_le(data, count), read_float_le(data, count));
    if (interpolation_type > 0) {
        truck.inTan = scaling(read_float_le(data, count), read_float_le(data, count), read_float_le(data, count));
        truck.outTan = scaling(read_float_le(data, count), read_float_le(data, count), read_float_le(data, count));
    }
    tracks.push_back(truck);
}
