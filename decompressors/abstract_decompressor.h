//
// Created by zealot on 05.01.2023.
//

#ifndef WAR3_MAP_PARSER_ABSTRACT_DECOMPRESSOR_H
#define WAR3_MAP_PARSER_ABSTRACT_DECOMPRESSOR_H


class abstract_decompressor {
public:
    virtual void decompress(char *in, char *out, unsigned av_in, unsigned av_out) = 0;
    static abstract_decompressor* get_decompressor(unsigned char compression_type);
};


#endif //WAR3_MAP_PARSER_ABSTRACT_DECOMPRESSOR_H
