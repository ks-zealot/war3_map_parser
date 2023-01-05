//
// Created by zealot on 05.01.2023.
//


#include <cassert>
#include "../data_objects/war3_inner_object/tileset_entry.h"
#include "../map_parser/util.h"

int main() {
    unsigned char arr[] = {0x51, 0x21, 0x00, 0x62, 0x56, 0x84, 0x13};
    unsigned char* unpacked_data = arr;
    tileset_entry entry;
    entry.height = read_int_16_le(unpacked_data);
    assert(entry.height = 8529);
    unpacked_data += 2;
    uint16_t data = read_int_16_le(unpacked_data);
    unpacked_data += 2;
    entry.water_level = 0x6200 & 0x3FFF;
    assert(entry.water_level = 8704);
    entry.is_boundary = data & 0xC000;
    unsigned char next = *unpacked_data++;
    unsigned char low = next & 0x0F;
    unsigned char high = next & 0xF0;
    entry.is_camera_boundary = high & 0x0080;
    entry.is_ramp = high & 0x0010;
    entry.is_water = high & 0x0040;
    assert(entry.is_ramp);
    assert(entry.is_water);
    entry.is_undead = high & 0x0020;
    entry.texture_type = low; // ref to tileset table
    assert(entry.texture_type = 6);
    entry.texture_details = *unpacked_data++;
    assert(entry.texture_details = 132);
    unsigned char next_ = *unpacked_data++;
    unsigned char low_ = next_ & 0x0F;
    unsigned char high_ = (next_  & 0xF0) >> 4;
    entry.cliff_type = high_;
    entry.tilepoint = low_;
    assert(entry.cliff_type = 1);
    assert(entry.tilepoint = 3);
    entry.final_height = (entry.height - 0x2000 + (entry.tilepoint - 2) * 0x0200) / 4;
    assert(entry.final_height = 212.25f);
    return 0;
}