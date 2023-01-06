//
// Created by zealot on 05.01.2023.
//

#ifndef WAR3_MAP_PARSER_TILESET_ENTRY_H
#define WAR3_MAP_PARSER_TILESET_ENTRY_H


#include <cstdint>
//Flags values (shown as big endian):
//0x4000 --> boundary flag 1 (shadow generated by the world editor on the edge of the map)
//0x0010 --> ramp flag (used to set a ramp between two layers)
//0x0020 --> blight flag (ground will look like Undead's ground)
//0x0040 --> water flag (enable water)
//0x0080 --> boundary flag 2 (used on "camera bounds" area. Usually set by the World Editor "boundary" tool.)

class tileset_entry {
    friend class enviroment_file_map_parser;
public://todo its for test purpose, remove
    int16_t height;
    int16_t water_level;
    uint16_t texture_type;

    unsigned char texture_details;
    unsigned char cliff_type;
    unsigned char tilepoint;

    bool is_boundary;
    bool is_ramp;
    bool is_undead;
    bool is_water;
    bool is_camera_boundary;


    unsigned final_height;
};


#endif //WAR3_MAP_PARSER_TILESET_ENTRY_H
