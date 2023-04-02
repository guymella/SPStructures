#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iBlock
    @ingroup Interfaces
    @brief interface for Procedural Memory Block
	
*/
#include "iCountable.h"
#include "iIterable.h"


class iBlock : public iCountable, public iIterable<uint8_t>
{
public:	
	//Size of table
	virtual size_t Size() const override = 0;
	// Copy Assignment
	virtual void operator=(const iBlock& rhs) { Copy(rhs); };
	/// C++ conform begin
	virtual Itr<uint8_t> begin(const int64_t& offset = 0) override { return Itr<uint8_t>(memStart() + offset); };
	/// C++ conform begin
	virtual Itr<const uint8_t> begin(const int64_t& offset = 0) const override { return Itr<const uint8_t>(memStart() + offset); };
	/// C++ conform end
	virtual Itr<uint8_t> end(const int64_t& offset = 0) override { return begin(Size() + offset); };
	/// C++ conform end
	virtual Itr<const uint8_t> end(const int64_t& offset = 0) const override { return begin(Size() + offset); };

	

protected:
	//Copy From Another Block
	virtual void Copy(const iBlock& rhs);
	//first address of blocks memory
	virtual inline uint8_t* memStart() = 0;
	//first address of blocks memory
	virtual inline const uint8_t* memStart() const = 0;

	
	
private:
};





void iBlock::Copy(const iBlock& rhs)
{
	if (Size() && rhs.Size())
		std::memcpy(begin(), rhs.begin(), ((rhs.Size()) < Size()) ? rhs.Size() : Size());
}



