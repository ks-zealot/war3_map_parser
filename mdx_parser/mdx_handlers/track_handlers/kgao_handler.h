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


    const inline std::vector<kgao_track> get_tracks() {
        return tracks;
    }
private:
    int get_tag() override;

    void parse_track() override;
    std::vector<kgao_track> tracks;
};


#endif //WAR3_MAP_PARSER_KGAO_HANDLER_H
