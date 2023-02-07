//
// Created by zealot on 01.02.2023.
//

#include "doodads_csv_parser.h"


void doodads_csv_parser::parse() {
    skip_header();

    while (csv.peek() != EOF) {
        doodads_entry de;
        de.id = read_cell();
        de.category = read_char();
        read_vector(de.tilesets);
        de.tileset_specific = read_bool();
        de.dir = read_cell();
        de.file = read_cell();
        de.comment = read_cell();
        de.name = read_cell();
        de.dood_class = read_cell();
        de.sound_loop = read_cell();
        de.sel_size = get_int();
        de.def_scale = read_float();
        de.min_scale = read_float();
        de.max_scale = read_float();
        de.can_place_rand_scale = read_bool();
        de.use_click_handler = read_bool();
        de.max_pitch = get_int_or_blank();
        de.max_roll = get_int_or_blank();
        de.vis_radius = get_int();
        de.walkable = read_bool();
        de.num_var = get_int();
        de.on_cliff = read_bool();
        de.on_water = read_bool();
        de.floats = read_bool();
        de.shadow = read_bool();
        de.show_in_fog = read_bool();
        de.anim_in_fog = read_bool();
        de.fixed_rot = get_int_or_blank();
        de.path_texture = read_cell();
        de.show_in_MM = read_bool();
        de.use_MM_color = read_bool();
        de.MM_red = get_int();
        de.MM_green = get_int();
        de.MM_blue = get_int();
        de.vertR01 = get_int();
        de.vertG01 = get_int();
        de.vertB01 = get_int();

        de.vertR02 = get_int();
        de.vertG02 = get_int();
        de.vertB02 = get_int();

        de.vertR03 = get_int();
        de.vertG03 = get_int();
        de.vertB03 = get_int();

        de.vertR04 = get_int();
        de.vertG04 = get_int();
        de.vertB04 = get_int();

        de.vertR05 = get_int();
        de.vertG05 = get_int();
        de.vertB05 = get_int();

        de.vertR06 = get_int();
        de.vertG06 = get_int();
        de.vertB06 = get_int();

        de.vertR07 = get_int();
        de.vertG07 = get_int();
        de.vertB07 = get_int();

        de.vertR08 = get_int();
        de.vertG08 = get_int();
        de.vertB08 = get_int();

        de.vertR09 = get_int();
        de.vertG09 = get_int();
        de.vertB09 = get_int();

        de.vertR10 = get_int();
        de.vertG10 = get_int();
        de.vertB10 = get_int();
        de.in_beta = read_bool();
         _map[de.id] = de;
    }
    csv.close();
}




