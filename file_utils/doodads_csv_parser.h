//
// Created by zealot on 01.02.2023.
//

#ifndef WAR3_MAP_PARSER_DOODADS_CSV_PARSER_H
#define WAR3_MAP_PARSER_DOODADS_CSV_PARSER_H

#include <string>
#include <fstream>
#include <unordered_map>
#include "csv_parser.h"
#include "csv_parser_structs.h"


class doodads_csv_parser: public csv_parser<doodads_entry> {
public:
    doodads_csv_parser(std::string file) : csv_parser(file) {};

    virtual void parse() override;

};


#endif //WAR3_MAP_PARSER_DOODADS_CSV_PARSER_H
