//
// Created by zealot on 01.02.2023.
//

#include <vector>
#include "doodads_csv_parser.h"


doodads_csv_parser::doodads_csv_parser(std::string file) {
    csv = std::ifstream(file);
}

void doodads_csv_parser::parse() {
    while (csv.peek() != 0x0A) {
        csv.get();
    }
    csv.get();

    while (csv.peek() != EOF) {
        doodads_entry de;
        de.id
    }
}


std::string doodads_csv_parser::read_cell() {
    std::vector<char> cell_store;
    while (csv.peek() != ';') {
        cell_store.push_back(csv.get());
    }
    csv.get();
    return std::string(cell_store.begin(), cell_store.end());
}

void doodads_csv_parser::read_vector(std::vector<std::string> &s) {
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

