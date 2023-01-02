//
// Created by zealot on 02.01.2023.
//

#include <iostream>
#include <thread>
#include <chrono>
#include "header_parser.h"
#include "util.h"

header_parser::header_parser(std::ifstream &map) : _map(map) {}

//Here is the header format (fixed size = 512 bytes):
//char[4]: file ID (should be "HM3W")
//int: unknown
//        string: map name
//int: map flags (these are exactly the same as the ones in the W3I file)
//0x0001: 1=hide minimap in preview screens
//0x0002: 1=modify ally priorities
//0x0004: 1=melee map
//0x0008: 1=playable map size was large and has never been reduced to medium
//0x0010: 1=masked area are partially visible
//0x0020: 1=fixed player setting for custom forces
//0x0040: 1=use custom forces
//0x0080: 1=use custom techtree
//0x0100: 1=use custom abilities
//0x0200: 1=use custom upgrades
//0x0400: 1=map properties menu opened at least once since map creation
//0x0800: 1=show water waves on cliff shores
//0x1000: 1=show water waves on rolling shores
//int: max number of players
//followed by 00 bytes until the 512 bytes of the header are filled.
//Here is the footer format (optional):
//char[4]: footer sign ID (should be "NGIS" == 'sign' reversed)
//byte[256]: 256 data bytes for authentification. I don't know how they are used
//at the moment.
void header_parser::parse() {
    read_file_id();
    read_unknown();
    read_map_name();
    read_flags();
    read_number_of_player();
    std::cout << "skip header... " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    _map.seekg(512);
}

void header_parser::read_number_of_player() {
  int sum = read_int_le(_map);
    std::cout << "number of player " << sum << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void header_parser::read_flags() {
    char *flags = new char[4];
    _map.read(flags, 4);
    delete[] flags;
}

void header_parser::read_map_name() const {
    std::string s;
    std::getline(_map, s, '\0');
    std::cout << "map name is " << s << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void header_parser::read_unknown() {
    char *unknown = new char[4];
    _map.read(unknown, 4);
    delete[] unknown;
}

void header_parser::read_file_id() {
    char *file_id = new char[4];
    _map.read(file_id, 4);
    std::string res = "HM3W";
    if (res != std::string(file_id, 4)) {
        throw std::runtime_error("Unknown file id");
    }
    std::cout << "file id is " << std::string(file_id, 4) << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    delete[] file_id;
}
