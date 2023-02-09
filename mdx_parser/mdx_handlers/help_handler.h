//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_HELP_HANDLER_H
#define WAR3_MAP_PARSER_HELP_HANDLER_H


#include "handler.h"
#include "../data/node.h"

struct helper {
    node node;
};

class help_handler : public handler {
public:
    help_handler(char *data, unsigned size) : handler(data, size) {};

    virtual void parse() override;

private:
    std::vector<helper> helpers;
};


#endif //WAR3_MAP_PARSER_HELP_HANDLER_H
