//
// Created by zealot on 07.02.2023.
//

#ifndef WAR3_MAP_PARSER_DESTRUCTABLE_UNIT_PARSER_H
#define WAR3_MAP_PARSER_DESTRUCTABLE_UNIT_PARSER_H


#include "csv_parser.h"
#include "csv_parser_structs.h"


class destructable_unit_parser : public csv_parser<destructable_unit_entry> {
public:
    destructable_unit_parser(std::string file) : csv_parser(file) {};

    virtual void parse() override;

};


#endif //WAR3_MAP_PARSER_DESTRUCTABLE_UNIT_PARSER_H
