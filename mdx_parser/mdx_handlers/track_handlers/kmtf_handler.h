//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_KMTF_HANDLER_H
#define WAR3_MAP_PARSER_KMTF_HANDLER_H


#include "track_handler.h"

#define KMTF 1179929931

class kmtf_handler : public track_handler {
public:
    kmtf_handler(char *&data, unsigned& count) : track_handler(data, count) {};


    const inline kmtf_track_data get_track_data() {
        return track_data;
    }

    void parse() override;

private:
    int get_tag() override;


    void parse_track() override;

    kmtf_track_data track_data;

};


#endif //WAR3_MAP_PARSER_KMTF_HANDLER_H
