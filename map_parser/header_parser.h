//
// Created by zealot on 02.01.2023.
//

#ifndef WAR3_MAP_PARSER_HEADER_PARSER_H
#define WAR3_MAP_PARSER_HEADER_PARSER_H


#include <fstream>

class header_parser {

public:
    explicit header_parser(std::ifstream &map);
public:
void parse();
private:
    std::ifstream& _map;

    void read_file_id();

    void read_unknown();

    void read_map_name() const;

    void read_flags();

    void read_number_of_player();
};


#endif //WAR3_MAP_PARSER_HEADER_PARSER_H
