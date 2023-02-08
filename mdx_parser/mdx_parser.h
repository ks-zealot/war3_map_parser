//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_MDX_PARSER_H
#define WAR3_MAP_PARSER_MDX_PARSER_H


#include <fstream>
#include <unordered_map>
#include "mdx_handlers/handler.h"

#define VERS 0x56455253
#define MODL 0x4D4F444C
#define SEQS 0x53455153
#define MTLS 0x4D544C53
#define TEXS 0x54455853
#define GEOS 0x47454F53
#define GEOA 0x47454F41
#define BONE 0x424F4E45
#define HELP 0x48454C50
#define PIVT 0x50495654

class mdx_parser {
public:
    mdx_parser(std::string f) : file(f) {};

    void parse();

private:
    std::ifstream file;

    handler*  get_handler(unsigned id, char* data, int size);
};


#endif //WAR3_MAP_PARSER_MDX_PARSER_H
