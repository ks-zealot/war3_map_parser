//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_KMTF_HANDLER_H
#define WAR3_MAP_PARSER_KMTF_HANDLER_H


#include "track_handler.h"

#define KMTF 0x4B4D5446

class kmtf_handler  : public track_handler{
public:
    int get_tag() override;

    void parse_track() override;

private:
    std::vector<kmtf_truck> tracks;

};


#endif //WAR3_MAP_PARSER_KMTF_HANDLER_H
