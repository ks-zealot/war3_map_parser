//
// Created by zealot on 02.01.2023.
//

#ifndef WAR3_MAP_PARSER_MPQ_PARSER_H
#define WAR3_MAP_PARSER_MPQ_PARSER_H


#include <fstream>

class mpq_parser {
public:
    explicit mpq_parser(std::ifstream &map);
public:
    void parse();
private:
    std::ifstream& _map;

    void parse_header();

    void read_block(int i);
};


#endif //WAR3_MAP_PARSER_MPQ_PARSER_H
