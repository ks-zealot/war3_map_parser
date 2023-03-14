//
// Created by zealot on 16.02.2023.
//

#ifndef WAR3_MAP_PARSER_MDX_MODEL_H
#define WAR3_MAP_PARSER_MDX_MODEL_H


#include <vector>
#include "mdx_handlers/geos_handler.h"
#include "mdx_handlers/texs_handler.h"

class mdx_model {
public:
    std::vector<geoset> geosets;
    std::vector<texture> textures;
};


#endif //WAR3_MAP_PARSER_MDX_MODEL_H
