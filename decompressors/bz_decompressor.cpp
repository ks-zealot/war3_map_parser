//
// Created by zealot on 05.01.2023.
//

#include <bzlib.h>
#include <stdexcept>
#include "bz_decompressor.h"

void bz_decompressor::decompress(char *in, char *out, unsigned int av_in, unsigned int av_out) {
    bz_stream strm;
    int nResult;

    // Initialize the BZIP2 decompression
    strm.next_in = in;
    strm.avail_in = av_in;
    strm.next_out = out;
    strm.avail_out = av_out;
    strm.bzalloc = NULL;
    strm.bzfree = NULL;
    strm.opaque = NULL;

    // Initialize decompression
    if ((nResult = BZ2_bzDecompressInit(&strm, 0, 0)) == BZ_OK) {
        // Perform the decompression
        nResult = BZ2_bzDecompress(&strm);
//        *pcbOutBuffer = strm.total_out_lo32;
        BZ2_bzDecompressEnd(&strm);
    }
    if (nResult >= BZ_OK) {
        return;
    }
    throw std::runtime_error("coud nt decompress");
}
