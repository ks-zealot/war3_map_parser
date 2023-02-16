//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_MDX_PARSER_H
#define WAR3_MAP_PARSER_MDX_PARSER_H


#include <fstream>
#include <unordered_map>
#include "mdx_handlers/handler.h"
#include "mdx_model.h"

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
#define GLBS 1396853831
#define TXAN 1312905300

class mdx_parser {
public:
    mdx_parser(std::string f) : file(f) {};

    void parse();

private:
    std::ifstream file;
    mdx_model model;

    void  handle_data(unsigned id, char* data, int size);

    void produce_model();
};


#endif //WAR3_MAP_PARSER_MDX_PARSER_H
