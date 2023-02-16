//
// Created by zealot on 15.02.2023.
//

#include "glbs_handler.h"
#include "../../map_parser/util.h"

void glbs_handler::parse() {
    for (int i = 0; i < size / 4; i++) {
        globalSequences.push_back(
                global_sequence(read_int_le(data), read_int_le(data), read_int_le(data), read_int_le(data)));
    }
}
