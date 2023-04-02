#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iBlock
    @ingroup Interfaces
    @brief interface for Procedural Memory Block
	
*/
#include "iBlock.h"
#include "Structures/Palloc.h"

class iBlockP : public iBlock {
public:
    
    
    virtual size_t Size() const override;
protected:
    virtual void copy(uint8_t Factor, uint8_t* Ptr) = 0;
    void move(iBlockP&& rhs);
    void reallocate(uint8_t Factor) { copy(Factor, (uint8_t*)Palloc(Factor)); };
    void free();
    void clear() { copy(0, nullptr); };
private:
    virtual uint8_t factor() const = 0;

    friend class iBlockD;
};

size_t iBlockP::Size() const
{
    if (memStart())
        return (uint64_t)1 << (factor());

    return 0;
}

inline void iBlockP::move(iBlockP&& rhs)
{
    copy(rhs.factor(), rhs.memStart()); rhs.clear();
}

inline void iBlockP::free()
{
    Palloc_Free(memStart());
    clear();
}




