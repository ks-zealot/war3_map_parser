//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_KGTR_HANDLER_H
#define WAR3_MAP_PARSER_KGTR_HANDLER_H


#include "track_handler.h"

#define KGTR 1381254987

class kgtr_handler : public track_handler {
public:
    kgtr_handler(char *&data, unsigned& count) : track_handler(data, count) {};


    const inline std::vector <kgtr_track> get_tracks() {
        return tracks;
    }

private:
    int get_tag() override;

    void parse_track() override;

    std::vector <kgtr_track> tracks;
};


#endif //WAR3_MAP_PARSER_KGTR_HANDLER_H
