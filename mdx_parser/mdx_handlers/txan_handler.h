//
// Created by zealot on 15.02.2023.
//

#ifndef WAR3_MAP_PARSER_TXAN_HANDLER_H
#define WAR3_MAP_PARSER_TXAN_HANDLER_H


#include "handler.h"

class txan_handler : public handler{
public:
    txan_handler(  char* data, unsigned size) : handler(data, size){};

    virtual void parse() override;
private:
    unsigned version;
};


#endif //WAR3_MAP_PARSER_TXAN_HANDLER_H
