//
// Created by zealot on 09.02.2023.
//

#include "geoa_handler.h"
#include "../../map_parser/util.h"

void geoa_handler::parse() {
    unsigned count = 0;
    while (count < size) {
        geoset_animation geo;
        unsigned local_count = 0;
        geo.inclusiveSize = read_int_le(data, local_count);
        geo.alpha = read_float_le(data, local_count);
        geo.flags = read_int_le(data, local_count);
        geo.color = color(read_float_le(data, local_count), read_float_le(data, local_count),
                          read_float_le(data, local_count));
        geo.geosetId = read_int_le(data, local_count);
        if (local_count < geo.inclusiveSize) {
            kgao_handler _kgao_handler(data, local_count);
            _kgao_handler.parse();
            geo.kgao_tracks = _kgao_handler.get_tracks();
        }
        if (local_count < geo.inclusiveSize) {
            kgac_handler _kgac_handler(data, local_count);
            _kgac_handler.parse();
            geo.kgac_tracks = _kgac_handler.get_tracks();
        }
        count += geo.inclusiveSize;
    }

}
