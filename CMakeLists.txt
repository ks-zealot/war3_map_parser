cmake_minimum_required(VERSION 3.24)
project(War3_Map_Parser)
find_package(BZip2)
find_package(ZLIB)
include_directories(${BZIP2_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})
 set(CMAKE_CXX_STANDARD 20)

add_executable(War3_Map_Parser main.cpp map_parser/map_parser.h map_parser/header_parser.h map_parser/header_parser.cpp map_parser/map_parser.cpp map_parser/mpq_parser.h map_parser/mpq_parser.cpp map_parser/util.h map_parser/util.cpp ${BZIP2_LIBRARIES} data_objects/block_table_entry.cpp data_objects/block_table_entry.h data_objects/hash_table_entry.cpp data_objects/hash_table_entry.h map_parser/abstract_map_file_parser.cpp map_parser/abstract_map_file_parser.h map_parser/enviroment_file_map_parser.cpp map_parser/enviroment_file_map_parser.h decompressors/abstract_decompressor.cpp decompressors/abstract_decompressor.h decompressors/bz_decompressor.cpp decompressors/bz_decompressor.h decompressors/zLib_decompressor.cpp decompressors/zLib_decompressor.h data_objects/war3_inner_object/tileset_entry.cpp data_objects/war3_inner_object/tileset_entry.h)
target_link_libraries (War3_Map_Parser PRIVATE BZip2::BZip2  ZLIB::ZLIB)
add_executable(test_tileset data_objects/war3_inner_object/tileset_entry.h map_parser/util.cpp   test/test_tileset.cpp data_objects/parser_inner_object/Vertex.h)
target_link_libraries(test_tileset  )

# enable testing functionality
enable_testing()

# define tests
add_test(
        NAME test_tileset
        COMMAND $<TARGET_FILE:test_tileset>
)