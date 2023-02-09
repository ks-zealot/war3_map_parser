//
// Created by zealot on 08.02.2023.
//

#include <string>
#include "seqs_handler.h"
#include "../../map_parser/util.h"

void seqs_handler::parse() {
    for (int i = 0; i < size / 132; i++) {
        sequence seq;
        seq.name = std::string(data, 80);
        data += 80;
        seq.interval.push_back(read_int_le(data));
        seq.interval.push_back(read_int_le(data));
        seq.move_speed = read_float_le(data);
        seq.flags = read_int_le(data);
        seq.rarity = read_float_le(data);
        seq.sync_point = read_int_le(data);

        seq.extent.boundsRadius = read_float_le(data);

        seq.extent.minimum.push_back(read_float_le(data));
        seq.extent.minimum.push_back(read_float_le(data));
        seq.extent.minimum.push_back(read_float_le(data));

        seq.extent.maximum.push_back(read_float_le(data));
        seq.extent.maximum.push_back(read_float_le(data));
        seq.extent.maximum.push_back(read_float_le(data));

        sequences.push_back(seq);
    }
    data -= 132 * sequences.size();
}
