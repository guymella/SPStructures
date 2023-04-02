#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Block
    @ingroup Block
    @brief inline Block of dynamic Memory, Self Allocating on heap
*/


#include "Interfaces/iBlockD.h"
#include "BlockPP.h"
//#include <string.h>
//#include <memory>

class BlockD : public iBlockD
{
public:
    BlockD() {};
    BlockD(uint64_t init_Size);
    virtual size_t Size() const override { return _size; };
private:
    BlockPP _block;
    uint64_t _size = 0;

    // Inherited via iBlockD

    virtual iBlockP* block() override { return &_block; };
    virtual const iBlockP* block() const override { return &_block; };
    virtual void setSize(uint64_t setSize) override { _size = setSize; };

};

BlockD::BlockD(uint64_t init_Size)
{
    Reserve(init_Size);
}

class BlockDR : public iBlockD
{
public:
    BlockDR() {};
    BlockDR(Pptr* block_Pptr, uint64_t * Size);
    virtual size_t Size() const override { return *_size; };
private:
    BlockPPR _block;
    uint64_t * _size;

    // Inherited via iBlockD
    virtual iBlockP* block() override { return &_block; };
    virtual const iBlockP* block() const override { return &_block; };
    virtual void setSize(uint64_t setSize) override { *_size = setSize; };
};

BlockDR::BlockDR(Pptr* block_Pptr, uint64_t* Size_ptr) : _block(block_Pptr), _size(Size_ptr)
{

}



