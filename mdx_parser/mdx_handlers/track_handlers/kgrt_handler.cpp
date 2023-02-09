//
// Created by zealot on 09.02.2023.
//

#include "kgrt_handler.h"
#include "../../../map_parser/util.h"

int kgrt_handler::get_tag() {
    return KGRT;
}

void kgrt_handler::parse_track() {
    kgrt_track truck;
    truck.frame = read_int_le(data);
    truck.value = rotation(read_float_le(data), read_float_le(data), read_float_le(data), read_float_le(data));
    if (interpolation_type > 0) {
        truck.inTan = rotation(read_float_le(data), read_float_le(data), read_float_le(data), read_float_le(data));
        truck.outTan = rotation(read_float_le(data), read_float_le(data), read_float_le(data), read_float_le(data));
    }
    tracks.push_back(truck);
}
