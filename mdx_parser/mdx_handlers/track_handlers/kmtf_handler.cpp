//
// Created by zealot on 08.02.2023.
//

#include "kmtf_handler.h"
#include "../../../map_parser/util.h"

int kmtf_handler::get_tag() {
    return KMTF;
}

void kmtf_handler::parse_track() {
    kmtf_truck truck;
    truck.frame = read_int_le(data);
    data += 4;
    truck.value = read_int_le(data);
    data += 4;
    if (interpolation_type > 0) {
        truck.inTan = read_int_le(data);
        data += 4;
        truck.outTan = read_int_le(data);
        data += 4;
    }
    tracks.push_back(truck);
}
