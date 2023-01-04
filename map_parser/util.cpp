//
// Created by zealot on 02.01.2023.
//
#include "util.h"

unsigned read_int_le(std::ifstream &map) {
    char *bytes = new char[4];
    int sum = 0;
    map.read(bytes, 4);
    sum += bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
    delete[] bytes;
    return sum;
}

uint16_t read_int_16_le(std::ifstream &map) {
    char *bytes = new char[2];
    int16_t sum = 0;
    map.read(bytes, 2);
    sum += bytes[0] | (bytes[1] << 8);
    delete[] bytes;
    return sum;
}

unsigned read_int_le(char *bytes) {
    int sum = 0;
    sum += bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
    return sum;
}

// Swaps a signed 16-bit integer
int16_t SwapInt16(uint16_t val)
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

// Swaps an unsigned 16-bit integer
uint16_t SwapUInt16(uint16_t val)
{
    return (val << 8) | (val >> 8 );
}

// Swaps a signed 32-bit integer
int32_t SwapInt32(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | ((val >> 16) & 0xFFFF);
}

// Swaps an unsigned 32-bit integer
uint32_t SwapUInt32(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}

// Swaps a signed 64-bit integer
int64_t SwapInt64(uint64_t val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

// Swaps an unsigned 64-bit integer
uint64_t SwapUInt64(uint64_t val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}

// Swaps array of unsigned 16-bit integers
void ConvertUInt16Buffer(void * ptr, size_t length)
{
    uint16_t * buffer = (uint16_t *)ptr;
    uint32_t nElements = (uint32_t)(length / sizeof(uint16_t));

    while(nElements-- > 0)
    {
        *buffer = SwapUInt16(*buffer);
        buffer++;
    }
}

// Swaps array of unsigned 32-bit integers
void ConvertUInt32Buffer(void * ptr, size_t length)
{
    uint32_t * buffer = (uint32_t *)ptr;
    uint32_t nElements = (uint32_t)(length / sizeof(uint32_t));

    while(nElements-- > 0)
    {
        *buffer = SwapUInt32(*buffer);
        buffer++;
    }
}

// Swaps array of unsigned 64-bit integers
void ConvertUInt64Buffer(void * ptr, size_t length)
{
    uint64_t * buffer = (uint64_t *)ptr;
    uint32_t nElements = (uint32_t)(length / sizeof(uint64_t));

    while(nElements-- > 0)
    {
        *buffer = SwapUInt64(*buffer);
        buffer++;
    }
}
