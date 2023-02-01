//
// Created by zealot on 01.02.2023.
//

#ifndef WAR3_MAP_PARSER_DOODADS_CSV_PARSER_H
#define WAR3_MAP_PARSER_DOODADS_CSV_PARSER_H

#include <string>
#include <fstream>
#include <unordered_map>
//doodID;category;tilesets;tilesetSpecific;dir;file;comment;name;doodClass;soundLoop;selSize;defScale;minScale;maxScale;canPlaceRandScale;useClickHelper;maxPitch;maxRoll;visRadius;walkable;numVar;onCliffs;onWater;floats;shadow;showInFog;animInFog;fixedRot;pathTex;showInMM;useMMColor;MMRed;MMGreen;MMBlue;vertR01;vertG01;vertB01;vertR02;vertG02;vertB02;vertR03;vertG03;vertB03;vertR04;vertG04;vertB04;vertR05;vertG05;vertB05;vertR06;vertG06;vertB06;vertR07;vertG07;vertB07;vertR08;vertG08;vertB08;vertR09;vertG09;vertB09;vertR10;vertG10;vertB10;InBeta

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
    float max_pitch;
    float max_roll;
    int vis_radius;

};
//todo вынести в абстрактный класс
class doodads_csv_parser {
public:
    doodads_csv_parser(std::string file);

    void parse();

    std::string doodad_name(std::string& id);

private:
    std::ifstream csv;
    std::unordered_map<std::string, doodads_entry> doodads_map;

    std::string read_cell();

    void read_vector(std::vector<std::string> &s);

};


#endif //WAR3_MAP_PARSER_DOODADS_CSV_PARSER_H
