#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Block
    @ingroup Block
    @brief inline Block of dynamic Memory, Self Allocating on heap
*/


#include "Interfaces/iBlockPP.h"
//#include <string.h>
//#include <memory>

class BlockPP : public iBlockPP
{
public:
    BlockPP() {};
    BlockPP(uint8_t Factor) { reallocate(Factor); };
    ~BlockPP() { free(); };
    BlockPP(BlockPP&& rhs) { _pptr = rhs._pptr; rhs.clear(); };
    BlockPP(iBlockP&& rhs) { move(std::move(rhs)); };
private:    
    Pptr _pptr;
    virtual Pptr* pptr() override { return &_pptr; };
    virtual const Pptr* pptr() const override { return &_pptr; };
};


class BlockPPR : public iBlockPP
{
public:
    BlockPPR() {};
    BlockPPR(Pptr* ptr) { _pptr = ptr; };
    ~BlockPPR() { _pptr = nullptr; };
private:
    Pptr* _pptr;
    virtual Pptr* pptr() override { return _pptr; };
    virtual const Pptr* pptr() const override { return _pptr; };
};









