//
// Created by zealot on 09.02.2023.
//

#include "geoa_handler.h"
#include "../../map_parser/util.h"

void geoa_handler::parse() {
    unsigned count = 0;
    while (count < size) {
        geoset_animation geo;
        geo.inclusiveSize = read_int_le(data);
        kgao_handler _kgao_handler(data);
        _kgao_handler.parse();
        geo.kgao_tracks = _kgao_handler.get_tracks();
        kgac_handler _kgac_handler(data);
        _kgac_handler.parse();
        geo.kgac_tracks = _kgac_handler.get_tracks();
        count += geo.inclusiveSize;
    }

}
