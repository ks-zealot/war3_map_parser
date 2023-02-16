//
// Created by zealot on 15.02.2023.
//

#ifndef WAR3_MAP_PARSER_GLBS_HANDLER_H
#define WAR3_MAP_PARSER_GLBS_HANDLER_H


#include "handler.h"

struct global_sequence {

    global_sequence(unsigned _1, unsigned _2, unsigned _3, unsigned _4) : _1(_1), _2(_2), _3(_3), _4(_4) {}

    unsigned _1;
    unsigned _2;
    unsigned _3;
    unsigned _4;
};

class glbs_handler : public handler {
public:
    glbs_handler(char *data, unsigned size) : handler(data, size) {};

    virtual void parse() override;

private:
    std::vector<global_sequence> globalSequences;


};


#endif //WAR3_MAP_PARSER_GLBS_HANDLER_H
