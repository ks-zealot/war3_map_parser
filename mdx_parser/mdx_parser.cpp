//
// Created by zealot on 08.02.2023.
//

#include <cassert>
#include <iostream>
#include "mdx_parser.h"
#include "../map_parser/util.h"
#include "mdx_handlers/vers_handler.h"
#include "mdx_handlers/seqs_handler.h"
#include "mdx_handlers/mtls_handler.h"
#include "mdx_handlers/texs_handler.h"
#include "mdx_handlers/geos_handler.h"
#include "mdx_handlers/geoa_handler.h"
#include "mdx_handlers/bone_handler.h"
#include "mdx_handlers/help_handler.h"
#include "mdx_handlers/pivt_handler.h"
#include "mdx_handlers/modl_handler.h"

void mdx_parser::parse() {
    char MDLX[4];
    file.read(MDLX, 4);
    std::string _MDLX(MDLX, 4);
    assert(_MDLX == "MDLX");
    while (file.peek() != EOF) {

        unsigned tag_as_int = read_int_le(file);
        unsigned size = read_int_le(file);
        char *data = new char[size];
        file.read(data, size);
        std::string _tag = std::string(static_cast<char *>(static_cast<void *>(&tag_as_int)), 4);
        std::cout << "handle " << _tag << " " << size << std::endl;
        handler *h = get_handler(tag_as_int, data, size);
        h->parse();
        delete[] data;
        delete h;
    }
}

handler *mdx_parser::get_handler(unsigned id, char *data, int size) {
    switch (id) {
        case VERS:
            return new vers_handler(data, size);
        case SEQS:
            return new seqs_handler(data, size);
        case MTLS:
            return new mtls_handler(data, size);
        case TEXS:
            return new texs_handler(data, size);
        case GEOS:
            return new geos_handler(data, size);
        case GEOA:
            return new geoa_handler(data, size);
        case BONE:
            return new bone_handler(data, size);
        case HELP:
            return new help_handler(data, size);
        case MODL:
            return new modl_handler(data, size);
        case PIVT:
            return new pivt_handler(data, size);
    }
}


 