#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Block
    @ingroup Block
    @brief inline Block of dynamic Memory, Self Allocating on heap
*/


#include "Interfaces/iBlockP.h"
#include "Structures/Palloc.h"
//#include <string.h>
//#include <memory>

class BlockP : public iBlockP
{
public:
    BlockP() {};
    BlockP(uint8_t Factor);
    ~BlockP();  
    BlockP(BlockP&& rhs) { move(std::move(rhs)); };
    BlockP(iBlockP&& rhs) { move(std::move(rhs)); };
protected:
    // Inherited via iBlockP
    virtual void copy(uint8_t Factor, uint8_t* Ptr) override;
private:
    uint64_t _factor = 0;
    uint8_t* _data = nullptr;

    // Inherited via iBlock    
    virtual uint8_t* memStart() override;
    virtual const uint8_t* memStart() const override;

    // Inherited via iBlockP
    virtual uint8_t factor() const override;

    
};



BlockP::BlockP(uint8_t Factor) : _factor(Factor)
{
    _data = (uint8_t*)Palloc(_factor);
}

BlockP::~BlockP()
{
    Palloc_Free(_data);
}

uint8_t* BlockP::memStart()
{
    return _data;
}

const uint8_t* BlockP::memStart() const
{
    return _data;
}

uint8_t BlockP::factor() const
{
    return _factor;
}

void BlockP::copy(uint8_t Factor, uint8_t* Ptr)
{
    _data = Ptr;
    _factor = Factor;
}


