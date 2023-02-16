//
// Created by zealot on 15.02.2023.
//

#ifndef WAR3_MAP_PARSER_SNDS_HANDLER_H
#define WAR3_MAP_PARSER_SNDS_HANDLER_H


#include <vector>
#include "handler.h"

struct sound {
    char data[272];
};

class snds_handler : public handler  {
public:
    snds_handler(char *data, unsigned size) : handler(data, size) {};

    virtual void parse() override;

private:
    std::vector<sound> sounds;
};


#endif //WAR3_MAP_PARSER_SNDS_HANDLER_H
