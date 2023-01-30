//
// Created by zealot on 30.01.2023.
//

#ifndef WAR3_MAP_PARSER_CSV_PARSER_H
#define WAR3_MAP_PARSER_CSV_PARSER_H


#include <fstream>
#include <unordered_map>
#include <vector>

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

class csv_parser {
public:
    csv_parser(std::string file);

    void parse();

    std::string texture_name(std::string& id);

private:
    std::ifstream csv;
    std::unordered_map<std::string, terrain_entry> terrain_map;

    std::string read_cell();

    void read_vector(std::vector<std::string> &s);

    std::string fix_file_separator(std::string& s);
};


#endif //WAR3_MAP_PARSER_CSV_PARSER_H
