//
// Created by zealot on 05.01.2023.
//

#include <zlib.h>
#include <stdexcept>
#include "zLib_decompressor.h"

void zLib_decompressor::decompress(char *in, char *out, unsigned int av_in, unsigned int av_out) {
    z_stream z;                        // Stream information for zlib
    int nResult;

    // Fill the stream structure for zlib
    z.next_in = (Bytef *) in;
    z.avail_in = (uInt) av_in;
    z.total_in = av_in;
    z.next_out = (Bytef *) out;
    z.avail_out = av_out;
    z.total_out = 0;
    z.zalloc = NULL;
    z.zfree = NULL;

    // Initialize the decompression structure. Storm.dll uses zlib version 1.1.3
    if ((nResult = inflateInit(&z)) == Z_OK) {
        // Call zlib to decompress the data
        nResult = inflate(&z, Z_FINISH);
//        *pcbOutBuffer = z.total_out;
        inflateEnd(&z);
    }

    if (nResult >= Z_OK) {
        return;
    }
    throw std::runtime_error("coud nt decompress");
}
