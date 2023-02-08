//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_SEQS_HANDLER_H
#define WAR3_MAP_PARSER_SEQS_HANDLER_H


#include "handler.h"
#include "../data/extent.h"

struct sequence {
    std::string name;
    std::vector<unsigned> interval;
    float move_speed;
    unsigned flags;// 0: looping
    // 1: non looping
    float rarity;
    unsigned sync_point;
    extent extent;
};

class seqs_handler : public handler {
public:
    seqs_handler(char *data, unsigned size) : handler(data, size) {};

    virtual void parse() override;

private:
    std::vector<sequence> sequences;

};


#endif //WAR3_MAP_PARSER_SEQS_HANDLER_H
