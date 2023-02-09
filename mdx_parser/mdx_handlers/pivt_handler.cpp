//
// Created by zealot on 09.02.2023.
//

#include "pivt_handler.h"
#include "../../map_parser/util.h"

void pivt_handler::parse() {
    for (int i = 0; i < size / 12; i++) {
        pivot p;
        p.x = read_float_le(data);
        p.y = read_float_le(data);
        p.z = read_float_le(data);
        pivots.push_back(p);
    }

}
