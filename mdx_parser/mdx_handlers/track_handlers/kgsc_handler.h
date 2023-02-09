//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_KGSC_HANDLER_H
#define WAR3_MAP_PARSER_KGSC_HANDLER_H


#include "track_handler.h"

#define KGTS  0x4475343

class kgsc_handler : public track_handler {
public:
    kgsc_handler(char *data) : track_handler(data) {};


    const inline std::vector <kgsc_track> get_tracks() {
        return tracks;
    }

private:
    int get_tag() override;

    void parse_track() override;

    std::vector <kgsc_track> tracks;
};


#endif //WAR3_MAP_PARSER_KGSC_HANDLER_H
