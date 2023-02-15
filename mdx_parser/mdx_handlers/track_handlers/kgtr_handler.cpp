//
// Created by zealot on 09.02.2023.
//

#include "kgtr_handler.h"
#include "../../../map_parser/util.h"

int kgtr_handler::get_tag() {
    return KGTR;
}

void kgtr_handler::parse_track() {
    kgtr_track truck;
    truck.frame = read_int_le(data, count);
    truck.value = translation(read_float_le(data, count), read_float_le(data, count), read_float_le(data, count));
    if (interpolation_type > 0) {
        truck.inTan = translation(read_float_le(data, count), read_float_le(data, count), read_float_le(data, count));
        truck.outTan = translation(read_float_le(data, count), read_float_le(data, count), read_float_le(data, count));
    }
    tracks.push_back(truck);
}
