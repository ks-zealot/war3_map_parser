//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_KMTA_HANDLER_H
#define WAR3_MAP_PARSER_KMTA_HANDLER_H


#include "track_handler.h"

#define KMTA 0x4B4D5441

class kmta_handler : public track_handler {
public:
    kmta_handler(char *data) : track_handler(data) {};


    const inline std::vector<kmta_truck> get_tracks() {
        return tracks;
    }
private:
    int get_tag() override;

    void parse_track() override;
    std::vector<kmta_track> tracks;
};


#endif //WAR3_MAP_PARSER_KMTA_HANDLER_H
