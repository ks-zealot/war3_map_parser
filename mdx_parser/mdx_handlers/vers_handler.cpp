//
// Created by zealot on 08.02.2023.
//

#include "vers_handler.h"
#include "../../map_parser/util.h"

void vers_handler::parse() {
    version = read_int_le(data);
}
