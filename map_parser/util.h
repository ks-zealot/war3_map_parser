//
// Created by zealot on 02.01.2023.
//

#ifndef WAR3_MAP_PARSER_UTIL_H
#define WAR3_MAP_PARSER_UTIL_H

#include <fstream>

int read_int_le(std::ifstream& map) {
    char *bytes = new char[4];
    int sum = 0;
    map.read(bytes, 4);
    sum += bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
    delete[] bytes;
    return sum;
}

int16_t read_int_16_le(std::ifstream& map) {
    char *bytes = new char[2];
    int16_t sum = 0;
    map.read(bytes, 2);
    sum += bytes[0] | (bytes[1] << 8)  ;
    delete[] bytes;
    return sum;
}


#endif //WAR3_MAP_PARSER_UTIL_H
