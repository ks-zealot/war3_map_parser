//
// Created by zealot on 05.01.2023.
//

#ifndef WAR3_MAP_PARSER_BZ_DECOMPRESSOR_H
#define WAR3_MAP_PARSER_BZ_DECOMPRESSOR_H


#include "abstract_decompressor.h"

class bz_decompressor  : public abstract_decompressor{
    void decompress(char *in, char *out, unsigned int av_in, unsigned int av_out) override;
};


#endif //WAR3_MAP_PARSER_BZ_DECOMPRESSOR_H
