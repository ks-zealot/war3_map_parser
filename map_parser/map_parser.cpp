//
// Created by zealot on 02.01.2023.
//

#include "map_parser.h"

map_parser::map_parser(std::ifstream &map) : _map(map), hp(map), mpqParser(map) {
}

void map_parser::parse() {
    hp.parse();
    mpqParser.parse();
}
