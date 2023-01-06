//
// Created by zealot on 05.01.2023.
//

#ifndef WAR3_MAP_PARSER_VERTEX_H
#define WAR3_MAP_PARSER_VERTEX_H


#include <cstdint>

class Vertex {
public:
    Vertex(uint16_t x, uint16_t y, uint16_t z) : x(x), y(y), z(z) {}

public:
    uint16_t x;
    uint16_t y;
    uint16_t z;

};


#endif //WAR3_MAP_PARSER_VERTEX_H
