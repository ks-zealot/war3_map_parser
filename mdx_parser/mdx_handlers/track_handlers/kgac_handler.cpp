//
// Created by zealot on 09.02.2023.
//

#include "kgac_handler.h"
#include "../../../map_parser/util.h"

int kgac_handler::get_tag() {
    return KGAC;
}

void kgac_handler::parse_track() {
    kgac_track truck;
    truck.frame = read_int_le(data);
    truck.value = color(read_float_le(data), read_float_le(data), read_float_le(data));
    if (interpolation_type > 0) {
        truck.inTan = color(read_float_le(data), read_float_le(data), read_float_le(data));
        truck.outTan = color(read_float_le(data), read_float_le(data), read_float_le(data));
    }
    tracks.push_back(truck);
}
