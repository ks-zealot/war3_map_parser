//
// Created by zealot on 30.01.2023.
//

#include <filesystem>
#include "csv_parser.h"
#include "csv_parser_structs.h"

template<typename T>
csv_parser<T>::csv_parser(std::string file) {
    csv = std::ifstream(file);

}

template<typename T>
std::string csv_parser<T>::read_cell() {
    std::vector<char> cell_store;
    while (csv.peek() != ';') {
        cell_store.push_back(csv.get());
    }
    csv.get();
    return std::string(cell_store.begin(), cell_store.end());
}

template<typename T>
void csv_parser<T>::read_vector(std::vector<std::string> &s) {
    std::vector<char> local;
    while (csv.peek() != ';') {
        local.push_back(csv.get());
        if (csv.peek() == ',' || csv.peek() == '.') {
            csv.get();
            s.push_back(std::string(local.begin(), local.end()));
        }
    }
    if (!local.empty()) {
        s.push_back(std::string(local.begin(), local.end()));
    }
    csv.get();
}

template<typename T>
std::string
csv_parser<T>::fix_file_separator(std::string &s) {
    std::replace(s.begin(), s.end(), '\\', std::filesystem::path::preferred_separator);
    return s;
}

template<typename T>
void csv_parser<T>::skip_header() {
    while (csv.peek() != 0x0A) {
        csv.get();
    }
    csv.get();
}

template<typename T>
char csv_parser<T>::read_char() {
    char c = csv.get();
    csv.get();
    return c;
}

template<typename T>
bool csv_parser<T>::read_bool() {
    bool b = read_char() == '1';
    return b;
}

template<typename T>
int csv_parser<T>::get_int() {
    int i = std::stoi(read_cell());
    return i;
}

template<typename T>
float csv_parser<T>::read_float() {
    return std::stof(read_cell());
}

template<typename T>
int csv_parser<T>::get_int_or_blank() {
    if (csv.peek() == '-') {
        while (csv.get() != ';') {}
        return -1;
    }
    return get_int();
}

template<typename T>
std::string csv_parser<T>::name(std::string &id) {
    if (!_map.contains(id)) {
        throw std::runtime_error("could not found texture");
    }
    T &entry = _map[id];
    std::string s = entry.dir + "\\" + entry.file + ".png";
    return fix_file_separator(s);
}

template
class csv_parser<destructable_unit_entry>;

template
class csv_parser<doodads_entry>;

template
class csv_parser<terrain_entry>;