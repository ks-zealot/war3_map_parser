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


    const inline kgtr_track_data get_track_data() {
        return track_data;
    }

    void parse() override;

private:
    int get_tag() override;

    void parse_track() override;

    kgtr_track_data track_data;
};


#endif //WAR3_MAP_PARSER_KGTR_HANDLER_H
