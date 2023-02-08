//
// Created by zealot on 08.02.2023.
//

#include <cassert>
#include <iostream>
#include "mdx_parser.h"
#include "../map_parser/util.h"
#include "mdx_handlers/vers_handler.h"

void mdx_parser::parse() {
    char MDLX[4];
    file.read(MDLX, 4);
    std::string _MDLX(MDLX, 4);
    assert(_MDLX == "MDLX");
    while (file.peek() != EOF) {
        char tag[4];
        file.read(tag, 4);
        std::string _tag(tag, 4);
        unsigned tag_as_int = read_int_le((unsigned char *) tag);
        unsigned size = read_int_le(file);
        char *data = new char[size];
        file.read(data, size);
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
            return new vers_handler((unsigned char *) data, size);
    }
}
