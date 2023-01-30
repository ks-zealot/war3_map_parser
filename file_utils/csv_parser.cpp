//
// Created by zealot on 30.01.2023.
//

#include <filesystem>
#include "csv_parser.h"

csv_parser::csv_parser(std::string file) {
    csv = std::ifstream(file);

}

void csv_parser::parse() {
    while (csv.peek() != 0x0A) {
        csv.get();
    }
    csv.get();

    while (csv.peek() != EOF) {
        terrain_entry entry;
        entry.tile_id = read_cell();
        entry.dir = read_cell();
        entry.file = read_cell();
        entry.comment = read_cell();
        entry.name = read_cell();
        entry.buildable = csv.get() == '1';
        csv.get();
        entry.footprints = csv.get() == '1';
        csv.get();
        entry.walkable = csv.get() == '1';
        csv.get();
        entry.flyable = csv.get() == '1';
        csv.get();
        entry.blightPri << csv.get() - '0';
        csv.get();
        read_vector(entry.convert_to);
        entry.in_beta = csv.get() == '1';
        csv.get();
        terrain_map[entry.tile_id] = entry;
    }

    csv.close();
}

std::string csv_parser::read_cell() {
    std::vector<char> cell_store;
    while (csv.peek() != ';') {
        cell_store.push_back(csv.get());
    }
    csv.get();
    return std::string(cell_store.begin(), cell_store.end());
}

void csv_parser::read_vector(std::vector<std::string> &s) {
    char buf[4] = {0, 0, 0, 0};
    while (csv.peek() != ';') {
        csv.read(buf, 4);
        s.push_back(std::string(buf, 4));
        if (csv.peek() == ',' || csv.peek() == '.') {
            csv.get();
        }
    }
    csv.get();
}

std::string csv_parser::texture_name(std::string &id) {
    if (!terrain_map.contains(id)) {
        throw std::runtime_error("could not found texture");
    }
    terrain_entry entry = terrain_map[id];
    std::string s = entry.dir + "\\" + entry.file + ".png";
    return fix_file_separator(s);
}

std::string
csv_parser::fix_file_separator(std::string& s) {
    std::replace( s.begin(), s.end(), '\\', std::filesystem::path::preferred_separator);
    return s;
}
