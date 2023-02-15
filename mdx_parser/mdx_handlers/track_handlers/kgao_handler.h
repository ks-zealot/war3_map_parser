//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_KGAO_HANDLER_H
#define WAR3_MAP_PARSER_KGAO_HANDLER_H


#include "track_handler.h"

#define KGAO 1329678155

class kgao_handler : public track_handler {
public:
    kgao_handler(char *&data, unsigned& count) : track_handler(data, count) {};


    const inline kgao_track_data get_tracks() {
        return track_data;
    }

    void parse() override;

private:
    int get_tag() override;

    void parse_track() override;

   kgao_track_data track_data;
};


#endif //WAR3_MAP_PARSER_KGAO_HANDLER_H
