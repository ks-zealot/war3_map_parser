//
// Created by zealot on 09.02.2023.
//

#include "kmta_handler.h"
#include "../../../map_parser/util.h"

int kmta_handler::get_tag() {
    return KMTA;
}

void kmta_handler::parse_track() {
    kmta_truck truck;
    truck.frame = read_int_le(data);
    truck.value = read_float_le(data);
    if (interpolation_type > 0) {
        truck.inTan = read_float_le(data);
        truck.outTan = read_float_le(data);
    }
    tracks.push_back(truck);
}
