//
// Created by zealot on 05.01.2023.
//

#include <stdexcept>
#include "abstract_decompressor.h"
#include "zLib_decompressor.h"
#include "bz_decompressor.h"

abstract_decompressor *abstract_decompressor::get_decompressor(unsigned char compression_type) {
    switch (compression_type) {
        case 0x02:
            return new zLib_decompressor();
        case 0x10:
            return new bz_decompressor();
    }
   throw std::runtime_error("unknon decompression type");
}
