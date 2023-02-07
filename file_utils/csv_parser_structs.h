//
// Created by zealot on 07.02.2023.
//

#ifndef WAR3_MAP_PARSER_CSV_PARSER_STRUCTS_H
#define WAR3_MAP_PARSER_CSV_PARSER_STRUCTS_H

#include <string>
#include <vector>

//DestructableID;category;tilesets;tilesetSpecific;dir;file;lightweight;fatLOS;texID;
//texFile;comment;name;doodClass;useClickHelper;onCliffs;onWater;canPlaceDead;walkable;
//cliffHeight;targType;armor;numVar;HP;occH;flyH;fixedRot;selSize;minScale;maxScale;canPlaceRandScale;
//maxPitch;maxRoll;radius;fogRadius;fogVis;pathTex;pathTexDeath;deathSnd;shadow;showInMM;useMMColor;MMRed;MMGreen;MMBlue;InBeta
struct destructable_unit_entry {
    std::string destructable_id;
    char category;
    std::vector<std::string> tilesets;
    bool tilesets_specific;
    std::string dir;
    std::string file;
    bool lightweight;
    bool fat_LOS;
    int texId;
    std::string tex_file;
    std::string comment;
    std::string name;
    char dood_class;
    bool use_click_helper;
    bool on_cliff;
    bool on_water;
    bool can_place_dead;
    bool walkable;
    int cliff_height;
    std::string targ_type;
    std::string armor;
    int num_var;
    int HP;
    int occH;
    int flyH;
    bool fixed_rot;
    int sel_size;
    float min_scale;
    float max_scale;
    bool can_place_rand_scale;
    int max_pitch;
    int max_roll;
    int radius;
    int fog_radius;
    int fog_vis;
    std::string path_tex;
    std::string path_tex_death;
    std::string death_snd;
    std::string shadow;
    bool show_in_MM;
    bool use_MM_color;
    bool MM_red;
    bool MM_green;
    bool MM_blue;
    bool in_beta;
};


//doodID;category;tilesets;tilesetSpecific;dir;file;comment;name;doodClass;soundLoop;selSize;defScale;minScale;maxScale;canPlaceRandScale;useClickHelper;maxPitch;maxRoll;visRadius;
//walkable;numVar;onCliffs;onWater;floats;shadow;showInFog;animInFog;fixedRot;pathTex;showInMM;useMMColor;
//MMRed;MMGreen;MMBlue;vertR01;vertG01;vertB01;vertR02;vertG02;vertB02;vertR03;vertG03;vertB03;vertR04;vertG04;vertB04;vertR05;vertG05;vertB05;vertR06;vertG06;vertB06;
//vertR07;vertG07;vertB07;vertR08;vertG08;vertB08;vertR09;vertG09;vertB09;vertR10;vertG10;vertB10;InBeta

struct doodads_entry {
    std::string id;
    char category;
    std::vector<std::string> tilesets;
    bool tileset_specific;
    std::string dir;
    std::string file;
    std::string comment;
    std::string name;
    std::string dood_class;
    std::string sound_loop;
    int sel_size;
    float def_scale;
    float min_scale;
    float max_scale;
    bool can_place_rand_scale;
    bool use_click_handler;
    int max_pitch;
    int max_roll;
    int vis_radius;
    bool walkable;
    int num_var;
    bool on_cliff;
    bool on_water;
    bool floats;
    bool shadow;
    bool show_in_fog;
    bool anim_in_fog;
    int fixed_rot;
    std::string path_texture;
    bool show_in_MM;
    bool use_MM_color;
    bool MM_red;
    bool MM_green;
    bool MM_blue;
    int vertR01;
    int vertG01;
    int vertB01;
    int vertR02;
    int vertG02;
    int vertB02;
    int vertR03;
    int vertG03;
    int vertB03;
    int vertR04;
    int vertG04;
    int vertB04;
    int vertR05;
    int vertG05;
    int vertB05;
    int vertR06;
    int vertG06;
    int vertB06;
    int vertR07;
    int vertG07;
    int vertB07;
    int vertR08;
    int vertG08;
    int vertB08;
    int vertR09;
    int vertG09;
    int vertB09;
    int vertR10;
    int vertG10;
    int vertB10;
    bool in_beta;
};

//tileID;dir;file;comment;name;buildable;footprints;walkable;flyable;blightPri;convertTo;InBeta
struct terrain_entry {

    std::string tile_id;
    std::string dir;
    std::string file;
    std::string comment;
    std::string name;
    bool buildable;
    bool footprints;
    bool walkable;
    bool flyable;
    int blightPri;
    std::vector<std::string> convert_to;
    bool in_beta;
};
#endif //WAR3_MAP_PARSER_CSV_PARSER_STRUCTS_H
