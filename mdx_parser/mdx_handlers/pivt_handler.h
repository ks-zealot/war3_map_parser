//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_PIVT_HANDLER_H
#define WAR3_MAP_PARSER_PIVT_HANDLER_H


#include "handler.h"

struct pivot {
    float x;
    float y;
    float z;
};

class pivt_handler : public handler {
public:
    pivt_handler(char *data, unsigned size) : handler(data, size) {};

    virtual void parse() override;

private:
    std::vector<pivot> pivots;
};


#endif //WAR3_MAP_PARSER_PIVT_HANDLER_H
