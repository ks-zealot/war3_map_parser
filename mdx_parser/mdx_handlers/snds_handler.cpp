//
// Created by zealot on 15.02.2023.
//

#include <cstring>
#include "snds_handler.h"

void snds_handler::parse() {
    for (int i = 0; i < size / 272; i++) {
        sound _sound;
        memcpy(_sound.data, data, 272);
        data += 272;
        sounds.push_back(_sound);
    }


}
