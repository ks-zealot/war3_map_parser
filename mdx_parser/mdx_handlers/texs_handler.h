//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_TEXS_HANDLER_H
#define WAR3_MAP_PARSER_TEXS_HANDLER_H

#include <vector>
#include <string>
#include "handler.h"

struct texture {
    unsigned replaceableId;
    std::string fileName;
    unsigned flags;
};

class texs_handler : public handler {
public:
    texs_handler(char *data, unsigned size) : handler(data, size) {};

    virtual void parse() override;

private:
    std::vector<texture> sequences;
};


#endif //WAR3_MAP_PARSER_TEXS_HANDLER_H
