//
// Created by zealot on 31.01.2023.
//

#ifndef WAR3_MAP_PARSER_TEXCOORD_H
#define WAR3_MAP_PARSER_TEXCOORD_H


class TexCoord {
public:
    TexCoord(float u, float w) : u(u), w(w) {}

public:
    float u;
    float w;
    float z;
};


#endif //WAR3_MAP_PARSER_TEXCOORD_H
