//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_KMTA_HANDLER_H
#define WAR3_MAP_PARSER_KMTA_HANDLER_H


#include "track_handler.h"

#define KMTA 1128351563

class kmta_handler : public track_handler {
public:
    kmta_handler(char *&data, unsigned& count) : track_handler(data, count) {};


    const inline kmta_track_data get_track_data() {
        return track_data;
    }

    void parse() override;

private:
    int get_tag() override;

    void parse_track() override;

    kmta_track_data track_data;

};


#endif //WAR3_MAP_PARSER_KMTA_HANDLER_H
