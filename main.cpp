#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "map_parser/map_parser.h"

int main() {
    std::ifstream map("/home/zealot/CLionProjects/War3_Map_Parser/(2)EchoIsles.w3m",  std::ios::binary );
    if (!map.is_open()) {
        return -1;
    }
    std::cout << "Open war 3 map (2)EchoIsles.w3m..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Parse header" << std::endl;
    map_parser parser(map);
    parser.parse();
    return 0;
}
