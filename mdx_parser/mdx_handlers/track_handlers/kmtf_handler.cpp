//
// Created by zealot on 08.02.2023.
//

#include "kmtf_handler.h"
#include "../../../map_parser/util.h"

int kmtf_handler::get_tag() {
    return KMTF;
}

void kmtf_handler::parse_track() {
    kmtf_track truck;
    truck.frame = read_int_le(data, count);
    truck.value = read_int_le(data, count);
    if (interpolation_type > 0) {
        truck.inTan = read_int_le(data, count);
        truck.outTan = read_int_le(data, count);
    }
    tracks.push_back(truck);
}
