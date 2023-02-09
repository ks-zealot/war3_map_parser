//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_VERS_HANDLER_H
#define WAR3_MAP_PARSER_VERS_HANDLER_H


#include "handler.h"

class vers_handler : public handler{
public:
    vers_handler(  char* data, unsigned size) : handler(data, size){};

    virtual void parse() override;
private:
    unsigned version;
};


#endif //WAR3_MAP_PARSER_VERS_HANDLER_H
