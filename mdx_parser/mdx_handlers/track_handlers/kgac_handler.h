//
// Created by zealot on 09.02.2023.
//

#ifndef WAR3_MAP_PARSER_KGAC_HANDLER_H
#define WAR3_MAP_PARSER_KGAC_HANDLER_H

#include "track_handler.h"

#define KGAC 1128351563

class kgac_handler : public track_handler {
public:
    kgac_handler(char *&data, unsigned& count) : track_handler(data, count) {};

    const inline std::vector<kgac_track> get_tracks() {
        return tracks;
    }

private:
    int get_tag() override;

    void parse_track() override;

    std::vector<kgac_track> tracks;
};

#endif //WAR3_MAP_PARSER_KGAC_HANDLER_H
