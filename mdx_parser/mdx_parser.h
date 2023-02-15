//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_MDX_PARSER_H
#define WAR3_MAP_PARSER_MDX_PARSER_H


#include <fstream>
#include <unordered_map>
#include "mdx_handlers/handler.h"

#define VERS 1397900630
#define MODL 1279545165
#define SEQS 1397835091
#define MTLS 1397511245
#define TEXS 1398293844
#define GEOS 1397704007
#define GEOA 1095714119
#define BONE 1162760002
#define HELP 1347175752
#define PIVT 1414941008

class mdx_parser {
public:
    mdx_parser(std::string f) : file(f) {};

    void parse();

private:
    std::ifstream file;

    handler*  get_handler(unsigned id, char* data, int size);
};


#endif //WAR3_MAP_PARSER_MDX_PARSER_H
