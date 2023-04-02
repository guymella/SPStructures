#pragma once
//------------------------------------------------------------------------------
/**
    @file Core/Types.h
    @brief defines basic data types for Oryol
*/
#include "Types.h"
#include <cstdlib>

void* Palloc(uint8_t factor) {
    uint64_t size = 1 << (factor);
    return _aligned_malloc(size, size);
}

void Palloc_Free(void * ptr) {
    _aligned_free(ptr);
}

uint8_t GetFactor(uint64_t size) {
    uint8_t factor = 0;
    while (size > 0) {
        size = size >> 1;
        factor++;
    }
    return factor;
}

class Pptr {
public:
    void Set(uint8_t Factor, uint8_t* ptr);
    uint8_t Factor() const;
    void Factor(uint8_t Factor);
    uint8_t*  Ptr() const;
    void  Ptr(uint8_t* Ptr);
private:
    uint64_t _pptr = 0;
};

void Pptr::Set(uint8_t Factor, uint8_t* Ptr)
{
    _pptr = Factor;
    _pptr = _pptr << 58;
    _pptr += (uint64_t)Ptr;
}

inline uint8_t Pptr::Factor() const
{
    return _pptr >> 58;
}

inline void Pptr::Factor(uint8_t Factor)
{
    Set(Factor, Ptr());
}

inline uint8_t* Pptr::Ptr() const
{
    return (uint8_t*)((_pptr << 6) >> 6);
}

inline void Pptr::Ptr(uint8_t* Ptr)
{
    Set(Factor(), Ptr);
}
