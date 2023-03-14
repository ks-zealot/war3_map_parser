//
// Created by zealot on 09.02.2023.
//

#include <cstring>
#include "texs_handler.h"
#include "../../map_parser/util.h"

void texs_handler::parse() {
    for (int i = 0; i < size / 268; i++) {
        texture t;
        t.replaceableId = read_int_le(data);
        t.fileName = std::string(data, 260);
        data += 260;
        t.flags = read_int_le(data);
        textures.push_back(t);
    }
}
