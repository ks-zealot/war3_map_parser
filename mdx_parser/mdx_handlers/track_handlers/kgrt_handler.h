//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_KGRT_HANDLER_H
#define WAR3_MAP_PARSER_KGRT_HANDLER_H


#include "track_handler.h"

#define KGRT 0x4B475254

class kgrt_handler : public track_handler {
public:
    kgrt_handler(char *data) : track_handler(data) {};


    const inline std::vector<kgrt_track> get_tracks() {
        return tracks;
    }

private:
    int get_tag() override;

    void parse_track() override;

    std::vector<kgrt_track> tracks;
};


#endif //WAR3_MAP_PARSER_KGRT_HANDLER_H
