//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_BONE_HANDLER_H
#define WAR3_MAP_PARSER_BONE_HANDLER_H


#include <string>
#include "handler.h"
#include "../data/track.h"
#include "../data/node.h"


struct bone {
    node node;
    unsigned geosetId;
    unsigned geosetAnimationId;
};

class bone_handler : public handler {
public:
    bone_handler(char *data, unsigned size) : handler(data, size) {};

    virtual void parse() override;

private:
    std::vector<bone> bones;
};


#endif //WAR3_MAP_PARSER_BONE_HANDLER_H
