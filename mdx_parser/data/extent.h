//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_EXTENT_H
#define WAR3_MAP_PARSER_EXTENT_H

#include <vector>

struct extent {
    float boundsRadius;
    std::vector<float> minimum;
    std::vector<float> maximum;
};
#endif //WAR3_MAP_PARSER_EXTENT_H
