//
// Created by zealot on 02.01.2023.
//

#ifndef WAR3_MAP_PARSER_UTIL_H
#define WAR3_MAP_PARSER_UTIL_H

#include <fstream>

unsigned read_int_le(std::ifstream &map);

unsigned read_int_le(char *&data);

float read_float_le(char *&data);

uint16_t read_int_16_le(std::ifstream &map);

uint16_t read_int_16_le(char *&data);


// Swaps a signed 16-bit integer
int16_t SwapInt16(uint16_t val);

// Swaps an unsigned 16-bit integer
uint16_t SwapUInt16(uint16_t val);

// Swaps a signed 32-bit integer
int32_t SwapInt32(uint32_t val);

// Swaps an unsigned 32-bit integer
uint32_t SwapUInt32(uint32_t val);

// Swaps a signed 64-bit integer
int64_t SwapInt64(uint64_t val);

// Swaps an unsigned 64-bit integer
uint64_t SwapUInt64(uint64_t val);

// Swaps array of unsigned 16-bit integers
void ConvertUInt16Buffer(void *ptr, size_t length);

// Swaps array of unsigned 32-bit integers
void ConvertUInt32Buffer(void *ptr, size_t length);

// Swaps array of unsigned 64-bit integers
void ConvertUInt64Buffer(void *ptr, size_t length);


#endif //WAR3_MAP_PARSER_UTIL_H
