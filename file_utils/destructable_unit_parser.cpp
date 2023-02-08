//
// Created by zealot on 07.02.2023.
//

#include <filesystem>
#include "destructable_unit_parser.h"


void destructable_unit_parser::parse() {
    skip_header();
    while (csv.peek() != EOF) {
        destructable_unit_entry due;
        due.destructable_id = read_cell();
        due.category = read_char();
        read_vector(due.tilesets);
        due.tilesets_specific = read_bool();
        due.dir = read_cell();
        due.file = read_cell();
        due.lightweight = read_bool();
        due.fat_LOS = read_bool();
        due.texId = get_int_or_blank();
        due.tex_file = read_cell();
        due.comment = read_cell();
        due.name = read_cell();
        due.dood_class = read_char();
        due.use_click_helper = read_bool();
        due.on_cliff = read_bool();
        due.on_water = read_bool();
        due.can_place_dead = read_bool();
        due.walkable = read_bool();
        due.cliff_height = get_int();
        due.targ_type = read_cell();
        due.armor = read_cell();
        due.num_var = get_int();
        due.HP = get_int();
        due.occH = get_int();
        due.flyH = get_int();
        due.fixed_rot = get_int_or_blank();
        due.sel_size = get_int();
        due.min_scale = read_float();
        due.max_scale = read_float();
        due.can_place_rand_scale = read_bool();
        due.max_roll = get_int_or_blank();
        due.max_pitch = get_int_or_blank();
        due.radius = get_int();
        due.fog_radius = get_int();
        due.fog_vis = get_int();
        due.tex_file = read_cell();
        due.path_tex_death = read_cell();
        due.death_snd = read_cell();
        due.shadow = read_cell();
        due.show_in_MM = read_bool();
        due.use_MM_color = read_bool();
        due.MM_red = get_int();
        due.MM_green = get_int();
        due.MM_blue = get_int();
        due.in_beta = read_bool();
        _map[due.destructable_id] = due;
    }
    csv.close();
}

