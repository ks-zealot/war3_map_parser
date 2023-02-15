//
// Created by zealot on 08.02.2023.
//

#ifndef WAR3_MAP_PARSER_TRACK_H
#define WAR3_MAP_PARSER_TRACK_H


struct translation {
    translation() : translation(-1) {};

    translation(float f) : x(f), y(f), z(f) {};

    translation(float x, float y, float z) : x(x), y(y), z(z) {};

    float x;
    float y;
    float z;
};

struct color {
    color() : color(-1) {};

    color(float f) : r(f), g(f), b(f) {};

    color(float r, float g, float b) : r(r), g(g), b(b) {};

    float r;
    float g;
    float b;
};

struct scaling {
    scaling() : scaling(-1) {};

    scaling(float f) : x(f), y(f), z(f) {};

    scaling(float x, float y, float z) : x(x), y(y), z(z) {};

    float x;
    float y;
    float z;
};

struct rotation {
    rotation() : rotation(-1) {};

    rotation(float f) : w(f), x(f), y(f), z(f) {};

    rotation(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {};
    float w;
    float x;
    float y;
    float z;
};

template<typename T>
struct track {
    int frame; // Probably should be uint32, but I saw a model with negative values
    T value;
    T inTan = T(-1);
    T outTan = T(-1);
};

template<typename T>
struct track_data {
    unsigned interpolationType; // 0: none
    // 1: linear
    // 2: hermite
    // 3: bezier
    unsigned globalSequenceId;
    std::vector<T> tracks;
};

struct kmtf_track : public track<int> {

};
struct kmta_track : public track<float> {

};
struct kgao_track : public track<float> {

};
struct kgac_track : public track<color> {

};
struct kgtr_track : public track<translation> {

};
struct kgrt_track : public track<rotation> {

};
struct kgsc_track : public track<scaling> {

};

struct kmtf_track_data : public track_data<kmtf_track> {

};
struct kmta_track_data : public track_data<kmta_track> {

};
struct kgao_track_data : public track_data<kgao_track> {

};
struct kgac_track_data : public track_data<kgac_track> {

};
struct kgtr_track_data : public track_data<kgtr_track> {

};
struct kgrt_track_data : public track_data<kgrt_track> {

};
struct kgsc_track_data : public track_data<kgsc_track> {

};
#endif //WAR3_MAP_PARSER_TRACK_H
