//
// Created by zealot on 05.01.2023.
//

#include "enviroment_file_map_parser.h"

void enviroment_file_map_parser::parse() {
        unsigned bytes_to_read = bp.is_file_compress ? bp.file_comp_size : bp.file_size;
        char * data = new char[bytes_to_read];
        _map.read(data, bytes_to_read);

}

