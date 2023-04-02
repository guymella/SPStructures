#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iBlock
    @ingroup Interfaces
    @brief interface for Procedural Memory Block
	
*/
#include "iBlockP.h"
#include "Structures/Palloc.h"

class iBlockPP : public iBlockP {
public:
    
    
protected:
    // Inherited via iBlockP
    virtual void copy(uint8_t Factor, uint8_t* Ptr) override;    
private:
    virtual uint8_t factor() const override;
    virtual Pptr* pptr() = 0;
    virtual const Pptr* pptr() const = 0;

    // Inherited via iBlock    
    virtual uint8_t* memStart() override;
    virtual const uint8_t* memStart() const override;

    
};

uint8_t iBlockPP::factor() const
{
    return pptr()->Factor();
}

uint8_t* iBlockPP::memStart()
{
    return pptr()->Ptr();
}

const uint8_t* iBlockPP::memStart() const
{
    return pptr()->Ptr();
}

void iBlockPP::copy(uint8_t Factor, uint8_t* Ptr)
{
    pptr()->Set(Factor, Ptr);
}
