//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_TRACK_HANDLER_H
#define WAR3_MAP_PARSER_TRACK_HANDLER_H


#include <vector>
#include "../../data/track.h"

class track_handler {
public:
    track_handler(char *&data, unsigned& count) : data(data), count(count) {};

    virtual void parse();

protected:
    char *&data;
    unsigned& count;
    unsigned tag;
    unsigned tracks_count;
    unsigned interpolation_type;// 0: none
    // 1: linear
    // 2: hermite
    // 3: bezier
    unsigned global_sequece_id;


    virtual void parse_track() = 0;
    virtual int get_tag() = 0;

};


#endif //WAR3_MAP_PARSER_TRACK_HANDLER_H
