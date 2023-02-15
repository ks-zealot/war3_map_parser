//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_KGSC_HANDLER_H
#define WAR3_MAP_PARSER_KGSC_HANDLER_H


#include "track_handler.h"

#define KGSC  1128351563

class kgsc_handler : public track_handler {
public:
    kgsc_handler(char *&data, unsigned &count) : track_handler(data, count) {};


    const inline kgsc_track_data get_track_data() {
        return track_data;
    }

    void parse() override;

private:
    int get_tag() override;

    void parse_track() override;

    kgsc_track_data track_data;
};


#endif //WAR3_MAP_PARSER_KGSC_HANDLER_H
