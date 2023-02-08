//
// Created by zealot on 30.01.2023.
//

#ifndef WAR3_MAP_PARSER_CSV_PARSER_H
#define WAR3_MAP_PARSER_CSV_PARSER_H


#include <fstream>
#include <unordered_map>
#include <vector>

template<typename T>
class csv_parser {
public:
    csv_parser(std::string file);

    virtual void parse() = 0;

    bool has (std::string& id);

   virtual std::string name(std::string& id);

   T& get(std::string& id);
    std::string fix_file_separator(std::string &s);

protected:
    std::ifstream csv;

    std::string read_cell();

    char read_char();

    bool read_bool();

    int get_int();

    int get_int_or_blank();

    float read_float();

    void read_vector(std::vector<std::string> &s);



    void skip_header();

    std::unordered_map<std::string, T> _map;
};




#endif //WAR3_MAP_PARSER_CSV_PARSER_H
