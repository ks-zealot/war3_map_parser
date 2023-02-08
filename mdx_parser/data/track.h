//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_TRACK_H
#define WAR3_MAP_PARSER_TRACK_H


template<typename T>
struct track  {
    int frame; // Probably should be uint32, but I saw a model with negative values
    T value;
    T inTan = T(-1);
    T outTan = T(-1);
};
struct kmtf_truck : public track<int> {

};
struct kmta_truck : public track<float> {

};
#endif //WAR3_MAP_PARSER_TRACK_H
