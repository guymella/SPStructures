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

#include <algorithm>


class iBlock : public iCountable, public iDynamicCap, public iIterable<uint8_t> {
public:
	virtual inline void* memStart() = 0;
	virtual inline const void* memStart() const = 0;
	//Total Capacity of container without reallocation
	virtual size_t Capacity() const override = 0;
	virtual size_t Size() const override = 0;
	/// total Amount of additional ellements that can be added without reallocation
	virtual size_t Spare() const { return Capacity() - Size(); };
	//Move memory left, truncates if past begin of block
	virtual void MoveLeft(size_t startOffset, size_t numElements, size_t shiftAmount);
	//Move Memory right, Truncates if past end of block
	virtual void MoveRight(size_t startOffset, size_t numElements, size_t shiftAmount);
	//move memory, truncates if past bounds of block 
	virtual void Move(size_t startOffset, size_t numElements, int64_t shiftAmount);
	//moves memory left and shuffles overwritten into vacated space, truncates if past beginning of block
	virtual void ShuffleLeft(size_t startOffset, size_t numElements, size_t shiftAmount);
	//moves memory right and shuffles overwritten into vacated space, truncates if past end of block
	virtual void ShuffleRight(size_t startOffset, size_t numElements, size_t shiftAmount);
	//moves memory and shuffles overwritten into vacated space, truncates if past ends of block
	virtual void Shuffle(size_t startOffset, size_t numElements, int64_t shiftAmount);
	//Overwrites memory with new elements 
	virtual void OverWrite(size_t startOffset, const uint8_t* rhs, size_t numElements);//truncates if block is static
	//inserts new memory shifts existing memory right
	virtual void Splice(size_t startOffset, const uint8_t* rhs, size_t numElements);//truncates if block is static
	//replaces specified section of memory with new elements, shifts remaining memory right
	virtual void Replace(size_t startOffset, size_t numElementRPL, const uint8_t* rhs, size_t numElementsCPY);//truncates if block is static
	//copies all data into this block, truncates if the coppied block is larger.
	virtual void Copy(const iBlock& rhs); //truncates if block is static
	//Compare this blocks memory to the specified memory, returns size of common prefix
	int64_t Compare(size_t offset, const uint8_t* rhs, size_t numElments) const;
	//campares this blocks memory to the specified and returns true if they are exactly equal
	bool Equal(size_t offset, const uint8_t* rhs, size_t numElments) const;


	/// copy-assignment operator (truncates if coppied block is larger
	virtual void operator=(const iBlock& rhs) { Copy(rhs); };

	virtual Itr<uint8_t> begin(const int64_t& offset = 0) override;
	virtual Itr<const uint8_t> begin(const int64_t& offset = 0) const override;
	virtual Itr<uint8_t> end(const int64_t& offset = 0) override ;
	virtual Itr<const uint8_t> end(const int64_t& offset = 0) const override;
private:
};

template <size_t SIZE>
class iStaticBlock : public iBlock {
	virtual size_t Size() const override { return SIZE; };
};
   

class iDBlock : public iBlock, public iGrowable, public iSmartGrowable {
public:
	virtual inline void* memStart() = 0;
	virtual inline const void* memStart() const = 0;
	//Total Capacity of container without reallocation
	virtual size_t Capacity() const override = 0;
	virtual size_t Size() const override = 0;
	// Grow Automatically
	virtual void Grow() override; 
	//grow to specific size, copy all to front
	virtual void Grow(const size_t& newSize) override; 
	//grow to specific size, copy all to front with offset
	virtual void Grow(const size_t& newSize, const size_t& frontPorch) override;
	//grow using CopyMap
	virtual void GrowMap(const CopyMap& map) override = 0;
};

void iBlock::Move(size_t startOffset, size_t numElements, int64_t shiftAmount)
{
	if (shiftAmount > 0)
		MoveRight(startOffset, numElements, shiftAmount);
	if (shiftAmount < 0)
		MoveLeft(startOffset, numElements, -shiftAmount);
}

void iBlock::Shuffle(size_t startOffset, size_t numElements, int64_t shiftAmount)
{
	if (shiftAmount > 0)
		ShuffleRight(startOffset, numElements, shiftAmount);
	if (shiftAmount < 0)
		ShuffleLeft(startOffset, numElements, -shiftAmount);
}

void iBlock::MoveLeft(size_t startOffset, size_t numElements, size_t shiftAmount)
{
	if (startOffset < shiftAmount) //truncate start
		startOffset += shiftAmount - startOffset;
	if (startOffset > Size())//will not Change this array
		return;
	if (startOffset + numElements > Size())
		numElements = Size() - startOffset; // rest of array

	memmove(begin(startOffset - shiftAmount), begin(startOffset), shiftAmount);
}

inline void iBlock::MoveRight(size_t startOffset, size_t numElements, size_t shiftAmount)
{
	if (startOffset + shiftAmount > Size()) //past end of block
		return;
	if (startOffset + numElements + shiftAmount > Size()) //truncate end
		numElements -= (startOffset + numElements + shiftAmount) - Size();

	memmove(begin(startOffset + shiftAmount), begin(startOffset), shiftAmount);
}

inline void iBlock::ShuffleLeft(size_t startOffset, size_t numElements, size_t shiftAmount)
{
	size_t shuffleStart = (startOffset > shiftAmount) ? startOffset - shiftAmount : 0;
	size_t shuffleNum = startOffset - shuffleStart;
	size_t shuffleShift = numElements;

	if (shuffleStart + shuffleShift > Size()) // will not change this array
		return;

	if (shuffleStart + shuffleNum + shuffleShift > Size()) //truncate end of shuffle
		shuffleNum -= (shuffleStart + shuffleNum + shuffleShift) - Size();

	//copy shuffle
	void* tmp = std::malloc(shuffleNum);
	std::memcpy(tmp, begin(shuffleStart), shuffleNum);
	//move range
	MoveLeft(startOffset, numElements, shiftAmount);
	//write shuffle
	std::memcpy(begin(shuffleStart + shuffleShift), tmp, shuffleNum);
	std::free(tmp);
}

inline void iBlock::ShuffleRight(size_t startOffset, size_t numElements, size_t shiftAmount)
{
	if (startOffset > Size())
		return;

	size_t shuffleStart = startOffset + numElements;
	size_t shuffleNum = shiftAmount;
	size_t shuffleShift = numElements;

	if (shuffleStart > Size()) // nothing to shuffle
		return MoveRight(startOffset, numElements, shiftAmount);

	if (shuffleStart + shuffleNum > Size())
		shuffleNum = Size() - shuffleStart;

	void* tmp = std::malloc(shuffleNum);
	std::memcpy(tmp, begin(shuffleStart), shuffleNum);

	MoveRight(startOffset, numElements, shiftAmount);

	std::memcpy(begin(startOffset), tmp, shuffleNum);
	std::free(tmp);
}

inline void iBlock::OverWrite(size_t startOffset, const uint8_t* src, size_t numElements)
{
	if (startOffset + numElements > Size())
		numElements -= (startOffset + numElements) - Size();
	std::memmove(begin(startOffset), src, numElements);
}

inline void iBlock::Splice(size_t startOffset, const uint8_t* rhs, size_t numElements)
{
	MoveRight(startOffset, Size(), numElements);
	OverWrite(startOffset, rhs, numElements);
}

inline void iBlock::Replace(size_t startOffset, size_t numElementRPL, const uint8_t* rhs, size_t numElementsCPY)
{
	MoveRight(startOffset+numElementRPL, Size(), numElementsCPY- numElementRPL);
	OverWrite(startOffset, rhs, numElementsCPY);
}

inline void iBlock::Copy(const iBlock& rhs)
{
	OverWrite(0, rhs.begin(), rhs.Size());
}

inline int64_t iBlock::Compare(size_t offset, const uint8_t* rhs, size_t numElments) const
{
	const uint8_t* bgn = begin(offset);
	auto r = std::mismatch(bgn, rhs, bgn+numElments);
	return (r.first[0] < r.second[0] ? -1:1)*(r.first - bgn);
}

inline bool iBlock::Equal(size_t offset, const uint8_t* rhs, size_t numElments) const
{
	return Compare(offset, rhs, numElments) == numElments;
}


Itr<uint8_t> iBlock::begin(const int64_t& offset)
{
	return Itr<uint8_t>((uint8_t*)memStart() + offset);
}

inline Itr<const uint8_t> iBlock::begin(const int64_t& offset) const
{
	return Itr<const uint8_t>((uint8_t*)memStart() + offset);
}



inline Itr<uint8_t> iBlock::end(const int64_t& offset)
{
	return begin(Size() + offset);
}

inline Itr<const uint8_t> iBlock::end(const int64_t& offset) const
{
	return begin(Size() + offset);
}



inline void iDBlock::Grow()
{
	Grow(Size() ? Size() * 2 : 2);
}
inline void iDBlock::Grow(const size_t& newSize)
{
	Grow(newSize, 0);
};

inline void iDBlock::Grow(const size_t& newSize, const size_t& frontPorch)
{
	//CopyRange r = { frontPorch, memStart(), Size() };
	CopyMap map = { newSize };
	//map.newSize = newSize;
	map.PushRange({ frontPorch, memStart(), Size() });
	GrowMap(map);
}

