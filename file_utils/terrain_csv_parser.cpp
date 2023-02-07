//
// Created by zealot on 07.02.2023.
//

#include "terrain_csv_parser.h"


void terrain_csv_parser::parse() {
    skip_header();

    while (csv.peek() != EOF) {
        terrain_entry entry;
        entry.tile_id = read_cell();
        entry.dir = read_cell();
        entry.file = read_cell();
        entry.comment = read_cell();
        entry.name = read_cell();
        entry.buildable = read_bool();
        entry.footprints =  read_bool();
        entry.walkable = read_bool();
        entry.flyable = read_bool();
        entry.blightPri = get_int();
        read_vector(entry.convert_to);
        entry.in_beta = read_bool();
        _map[entry.tile_id] = entry;
    }

    csv.close();
}

