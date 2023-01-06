//
// Created by zealot on 05.01.2023.
//

#ifndef WAR3_MAP_PARSER_VERTEX_H
#define WAR3_MAP_PARSER_VERTEX_H


#include <cstdint>

class Vertex {
public:
    Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

public:
    float x;
    float y;
    float z;

};


#endif //WAR3_MAP_PARSER_VERTEX_H
