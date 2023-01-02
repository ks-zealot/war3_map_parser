//
// Created by zealot on 02.01.2023.
//

#ifndef WAR3_MAP_PARSER_MAP_PARSER_H
#define WAR3_MAP_PARSER_MAP_PARSER_H


#include <fstream>
#include "header_parser.h"

class map_parser {
public:
    explicit map_parser(std::ifstream &map);


public:
    void parse();
private:
   std::ifstream& _map;
    header_parser hp;
};


#endif //WAR3_MAP_PARSER_MAP_PARSER_H
