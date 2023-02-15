//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_KGRT_HANDLER_H
#define WAR3_MAP_PARSER_KGRT_HANDLER_H


#include "track_handler.h"

#define KGRT 1414678347

class kgrt_handler : public track_handler {
public:
    kgrt_handler(char *&data, unsigned &count) : track_handler(data, count) {};


    const inline kgrt_track_data get_track_data() {
        return track_data;
    }

    void parse() override;

private:
    int get_tag() override;

    void parse_track() override;

    kgrt_track_data track_data;
};


#endif //WAR3_MAP_PARSER_KGRT_HANDLER_H
