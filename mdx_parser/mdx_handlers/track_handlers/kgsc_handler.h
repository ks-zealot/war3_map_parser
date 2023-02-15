//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_KGSC_HANDLER_H
#define WAR3_MAP_PARSER_KGSC_HANDLER_H


#include "track_handler.h"

#define KGSC  1128351563

class kgsc_handler : public track_handler {
public:
    kgsc_handler(char *&data, unsigned & count) : track_handler(data, count) {};


    const inline std::vector <kgsc_track> get_tracks() {
        return tracks;
    }

private:
    int get_tag() override;

    void parse_track() override;

    std::vector <kgsc_track> tracks;
};


#endif //WAR3_MAP_PARSER_KGSC_HANDLER_H
