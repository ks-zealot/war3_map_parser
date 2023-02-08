//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_MODL_HANDLER_H
#define WAR3_MAP_PARSER_MODL_HANDLER_H


#include <string>
#include "handler.h"
#include "../data/extent.h"


class modl_handler : public handler {
public:
    modl_handler(  char *data, unsigned size) : handler(data, size) {};

    virtual void parse() override;

private:
    std::string name;
    std::string animation_file_name;
    extent extent;
    unsigned blendTime;
};


#endif //WAR3_MAP_PARSER_MODL_HANDLER_H
