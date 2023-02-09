//
// Created by zealot on 09.02.2023.
//

#include "kgsc_handler.h"
#include "../../../map_parser/util.h"

int kgsc_handler::get_tag() {
    return KGTS;
}

void kgsc_handler::parse_track() {
    kgsc_track truck;
    truck.frame = read_int_le(data);
    truck.value = scaling(read_float_le(data), read_float_le(data), read_float_le(data));
    if (interpolation_type > 0) {
        truck.inTan = scaling(read_float_le(data), read_float_le(data), read_float_le(data));
        truck.outTan = scaling(read_float_le(data), read_float_le(data), read_float_le(data));
    }
    tracks.push_back(truck);
}
