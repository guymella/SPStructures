#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iBlock
    @ingroup Interfaces
    @brief interface for Procedural Memory Block
	
*/
#include "Structures/Types.h"
#include "iDynamic.h"
#include "iCountable.h"
#include "iIterable.h"


class iBlock : public iCountable, public iCIterable<uint8_t> {
public:
	virtual void* memStart() = 0;
	virtual const void* memStart() const = 0;


	virtual uint8_t* begin(const int64_t& offset = 0) override;
	/// C++ conform begin
	virtual const uint8_t* begin(const int64_t& offset = 0) const override;
	/// C++ conform end
	virtual uint8_t* end(const int64_t& offset = 0) override ;
	/// C++ conform end
	virtual const uint8_t* end(const int64_t& offset = 0) const override;

private:
};
   

class iDBlock : public iBlock, public iDynamicGrow {
public:
	//void SetType(baseTypes newType) override;	
	virtual void GrowCopyMap(const size_t& newSize, CopyRange* CopyMap, size_t CopyMapSize) = 0;
};






uint8_t* iBlock::begin(const int64_t& offset)
{
	return (uint8_t*)memStart() + offset;
}

inline const uint8_t* iBlock::begin(const int64_t& offset) const
{
	return (uint8_t*)memStart() + offset;
}

inline uint8_t* iBlock::end(const int64_t& offset)
{
	return begin(Size() + offset);
}

inline const uint8_t* iBlock::end(const int64_t& offset) const
{
	return begin(Size() + offset);
};
