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
    truck.frame = read_int_le(data, count);
    truck.value = rotation(read_float_le(data, count), read_float_le(data, count), read_float_le(data, count), read_float_le(data, count));
    if (interpolation_type > 0) {
        truck.inTan = rotation(read_float_le(data, count), read_float_le(data, count), read_float_le(data, count), read_float_le(data, count));
        truck.outTan = rotation(read_float_le(data, count), read_float_le(data, count), read_float_le(data, count), read_float_le(data, count));
    }
    tracks.push_back(truck);
}
