//
// Created by zealot on 01.02.2023.
//

#ifndef WAR3_MAP_PARSER_DOODADS_PARSER_H
#define WAR3_MAP_PARSER_DOODADS_PARSER_H


#include "abstract_map_file_parser.h"
#include "../data_objects/war3_inner_object/tree_entry.h"
#include "../file_utils/csv_parser.h"
#include "../file_utils/doodads_csv_parser.h"
#include "../file_utils/destructable_unit_parser.h"

class doodads_parser : public abstract_map_file_parser {
public:
    doodads_parser(block_table_entry &entry, std::ifstream &fstream) : abstract_map_file_parser(entry,
            fstream) {};

    doodads_parser(std::string file);

    virtual void parse() override;

    virtual void aggregate() override;
    virtual ~doodads_parser();
private:
    void read_block(const std::vector<unsigned int> &offset_table, int i);

    unsigned char *unpacked_data;

    unsigned int numbers_of_trees;
    unsigned int special_doodads;

    std::vector<tree_entry> trees;
    csv_parser<doodads_entry>*  _doodads_csv_parser = new doodads_csv_parser("/home/zealot/CLionProjects/War3_Map_Parser/assets/Doodads/Doodads.csv");
    csv_parser<destructable_unit_entry>* _destructable_unit_parser  = new destructable_unit_parser("/home/zealot/CLionProjects/War3_Map_Parser/assets/Units/DestructableData.csv");
};


#endif //WAR3_MAP_PARSER_DOODADS_PARSER_H
