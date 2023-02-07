//
// Created by zealot on 07.02.2023.
//

#ifndef WAR3_MAP_PARSER_TERRAIN_CSV_PARSER_H
#define WAR3_MAP_PARSER_TERRAIN_CSV_PARSER_H

#include <string>
#include <vector>
#include "csv_parser.h"
#include "csv_parser_structs.h"


class terrain_csv_parser : public csv_parser<terrain_entry> {
public:
    terrain_csv_parser(std::string file): csv_parser(file) {};

    std::string texture_name(std::string& id);

    virtual void parse() override;
};


#endif //WAR3_MAP_PARSER_TERRAIN_CSV_PARSER_H
