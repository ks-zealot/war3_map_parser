//
// Created by zealot on 01.02.2023.
//

#ifndef WAR3_MAP_PARSER_TREE_ENTRY_H
#define WAR3_MAP_PARSER_TREE_ENTRY_H

//float: Tree X coordinate on the map
//float: Tree Y coordinate on the map
//float: Tree Z coordinate on the map
//float: Tree angle (radian angle value)(degree = radian*180/pi)
//float: Tree X scale
//float: Tree Y scale
//float: Tree Z scale
//        byte: Tree flags*
//byte: Tree life (integer stored in %, 100% is 0x64, 170% is 0xAA for example)
//int: Random item table pointer
//if -1 -> no item table
//if >= 0 -> items from the item table with this number (defined in the w3i) are dropped on death
//int: number "n" of item sets dropped on death (this can only be greater than 0 if the item table pointer was -1)
//then there is n times a item set structure
//int: Tree ID number in the World Editor (little endian) (each tree has a different one)
class tree_entry {
public:
    float x;
    float y;
    float z;
    float angle;
    float scale_x;
    float scale_y;
    float scale_z;
    bool visible;
    bool solid;
    unsigned char health;
    int item_id;
    int item_count;
    int id;

};


#endif //WAR3_MAP_PARSER_TREE_ENTRY_H
