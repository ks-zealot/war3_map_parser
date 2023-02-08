//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_HANDLER_H
#define WAR3_MAP_PARSER_HANDLER_H

#include <vector>

class handler {
public:
    handler(  char *data, unsigned size) : data(data), size(size) {};

    virtual void parse() = 0;

protected:
    char *data;
    unsigned size;
};


#endif //WAR3_MAP_PARSER_HANDLER_H
