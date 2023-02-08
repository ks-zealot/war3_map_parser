//
// Created by zealot on 08.02.2023.
//

#include "modl_handler.h"
#include "../../map_parser/util.h"

void modl_handler::parse() {
    name = std::string(data, 80);
    data += 80;
    animation_file_name = std::string(data, 260);
    data += 260;
    extent.boundsRadius = read_float_le(data);
    data += 4;

    extent.minimum.push_back(read_float_le(data));
    data += 4;
    extent.minimum.push_back(read_float_le(data));
    data += 4;
    extent.minimum.push_back(read_float_le(data));
    data += 4;

    extent.maximum.push_back(read_float_le(data));
    data += 4;
    extent.maximum.push_back(read_float_le(data));
    data += 4;
    extent.maximum.push_back(read_float_le(data));
    data += 4;
    data -= 80;
    data -= 260;
    data -= 4 * 7;

}
