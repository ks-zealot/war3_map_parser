cmake_minimum_required(VERSION 3.24)
project(War3_Map_Parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(War3_Map_Parser main.cpp map_parser/map_parser.h map_parser/header_parser.h map_parser/header_parser.cpp map_parser/map_parser.cpp map_parser/mpq_parser.h map_parser/mpq_parser.cpp map_parser/util.h)
