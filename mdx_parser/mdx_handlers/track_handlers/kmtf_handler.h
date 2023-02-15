//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_KMTF_HANDLER_H
#define WAR3_MAP_PARSER_KMTF_HANDLER_H


#include "track_handler.h"

#define KMTF 1128351563

class kmtf_handler : public track_handler {
public:
    kmtf_handler(char *&data, unsigned& count) : track_handler(data, count) {};


    const inline std::vector<kmtf_track> get_tracks() {
        return tracks;
    }

private:
    int get_tag() override;

    void parse_track() override;

    std::vector<kmtf_track> tracks;

};


#endif //WAR3_MAP_PARSER_KMTF_HANDLER_H
