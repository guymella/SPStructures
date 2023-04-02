#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Block
    @ingroup Block
    @brief inline Block of dynamic Memory, Self Allocating on heap
*/


#include "Interfaces/iBlock.h"
//#include <string.h>
//#include <memory>

class Block : public iBlock
{
public:
    Block() {};
    Block(uint64_t Size);
    ~Block();
    virtual size_t Size() const override;
private:
    uint64_t _size = 0;
    uint8_t* _data = nullptr;

    // Inherited via iBlock
    
    virtual uint8_t* memStart() override;
    virtual const uint8_t* memStart() const override;
};



Block::Block(uint64_t Size) : _size(Size)
{
    _data = (uint8_t*)malloc(_size);
}

Block::~Block()
{
    free(_data);
}

size_t Block::Size() const
{
    return _size;
}

uint8_t* Block::memStart()
{
    return _data;
}

const uint8_t* Block::memStart() const
{
    return _data;
}