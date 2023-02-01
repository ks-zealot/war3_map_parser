//
// Created by zealot on 01.02.2023.
//

#ifndef WAR3_MAP_PARSER_DOODADS_PARSER_H
#define WAR3_MAP_PARSER_DOODADS_PARSER_H


#include "abstract_map_file_parser.h"
#include "../data_objects/war3_inner_object/tree_entry.h"

class doodads_parser : public abstract_map_file_parser {
public:
    doodads_parser(block_table_entry &entry, std::ifstream &fstream) : abstract_map_file_parser(entry,
            fstream) {};
    virtual void parse() override;

    virtual void aggregate() override;
    virtual ~doodads_parser();
private:
    void read_block(const std::vector<unsigned int> &offset_table, int i);

    unsigned char *unpacked_data;

    unsigned int numbers_of_trees;
    std::vector<tree_entry> trees;
};


#endif //WAR3_MAP_PARSER_DOODADS_PARSER_H
