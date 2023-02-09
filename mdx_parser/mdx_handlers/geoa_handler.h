//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_GEOA_HANDLER_H
#define WAR3_MAP_PARSER_GEOA_HANDLER_H


#include "handler.h"
#include "../data/color.h"
#include "track_handlers/kgao_handler.h"
#include "track_handlers/kgac_handler.h"

struct geoset_animation {
    unsigned inclusiveSize;
    float alpha;
    unsigned flags;
    color color;
    unsigned geosetId;
    std::vector<kgao_track> kgao_tracks;
    std::vector<kgac_track> kgac_tracks;
//(KGAO)
//        (KGAC)
};

class geoa_handler : public handler {
public:
    geoa_handler(char *data, unsigned size) : handler(data, size) {};

    virtual void parse() override;

private:
    std::vector<geoset_animation> geosets;


};


#endif //WAR3_MAP_PARSER_GEOA_HANDLER_H
