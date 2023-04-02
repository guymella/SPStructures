#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iBlock
    @ingroup Interfaces
    @brief interface for Procedural Memory Block
	
*/
#include "BlockP.h"
#include "iDynamic.h"


class iBlockD : public iBlock
{
public:	
	//Size of table
	virtual size_t Size() const override = 0;

	//Total Capacity of container without reallocation
	virtual size_t Capacity() const { return block()->Size();	};
	/// total Amount of additional ellements that can be added without reallocation
	virtual size_t Spare() const { return Capacity() - Size(); }; 	
	/// realocat and include at least numElements Spare At Back
	virtual void Reserve(const size_t& numElements);
	//shift elements to the right
	Itr<uint8_t> iBlockD::Shift_Right(uint64_t StartPos, uint64_t numElements, uint64_t shiftAmount);
	//shift elements to the left
	Itr<uint8_t> iBlockD::Shift_Left(uint64_t StartPos, uint64_t numElements, uint64_t shiftAmount);
	// insert n blank Elements in the block return iter at beginning of blank space
	Itr<uint8_t> Insert_Blank(uint64_t numElements, uint64_t StartPos = ULONG_MAX);
	// insert n blank Elements in the block return iter at beginning of blank space
	Itr<uint8_t> Remove(uint64_t numElements, uint64_t StartPos = ULONG_MAX);
	// copy one block into another return itr at end of inserted block
	void Insert(const iBlock * block, uint64_t StartPos = ULONG_MAX);
	// overite one block over existing data
	Itr<uint8_t> Replace_Blank(uint64_t StartPos = 0, uint64_t numElements = 0, uint64_t NewNumElements = 0);
	// copy one block into another return itr at end of inserted block
	void Replace(const iBlock* block, uint64_t StartPos = ULONG_MAX, uint64_t numElements = 0);

protected:
	

	
	
private:
	virtual iBlockP* block() = 0;
	virtual const iBlockP* block() const = 0;

	virtual void setSize(uint64_t setSize) = 0;

	//first address of blocks memory
	virtual inline uint8_t* memStart() override { return block()->memStart(); };
	//first address of blocks memory
	virtual inline const uint8_t* memStart() const override { return block()->memStart(); };
};


void iBlockD::Reserve(const size_t& numElements)
{
	if (Spare() < numElements)
	{
		BlockP old = std::move(*block());
		block()->reallocate(GetFactor(numElements+Size()));
		memcpy(block()->begin(), old.begin(), Size());		
	}
}
inline Itr<uint8_t> iBlockD::Shift_Right(uint64_t StartPos, uint64_t numElements, uint64_t shiftAmount)
{
	if (StartPos > Size())
		return end();
	if (StartPos + numElements + shiftAmount > Size()) {
		numElements -= StartPos + numElements + shiftAmount - Size();
	}

	memmove(begin(StartPos + shiftAmount), begin(StartPos), numElements);

	return begin(StartPos);
}
inline Itr<uint8_t> iBlockD::Shift_Left(uint64_t StartPos, uint64_t numElements, uint64_t shiftAmount)
{	
	if (StartPos + numElements > Size()) {
		numElements -= StartPos + numElements - Size();
	}
	if (StartPos - shiftAmount < 0)
		StartPos += shiftAmount - StartPos;

	memmove(begin(StartPos - shiftAmount), begin(StartPos), numElements);

	return begin(StartPos - shiftAmount);
}

inline Itr<uint8_t> iBlockD::Insert_Blank(uint64_t numElements, uint64_t StartPos)
{
	uint64_t size = Size();

	if (StartPos) 
		if (StartPos > size)
			StartPos = size;

	Reserve(numElements);
	setSize(size + numElements);

	Shift_Right(StartPos, size - StartPos, numElements);

	return begin(StartPos);
}

inline Itr<uint8_t> iBlockD::Remove(uint64_t numElements, uint64_t StartPos)
{
	if (StartPos == ULONG_MAX)
		StartPos = Size() - numElements;

	uint64_t size = Size();

	if (StartPos)
		if (StartPos > size)
			return end();

	if (StartPos + numElements > size)
		numElements = size - StartPos;

	Shift_Left(StartPos, size - StartPos, numElements);

	setSize(size - numElements);	

	return begin(StartPos);
}

inline void iBlockD::Insert(const iBlock* block, uint64_t StartPos)
{
	memmove(Insert_Blank(block->Size(), StartPos), block->begin(), block->Size());
}

inline Itr<uint8_t> iBlockD::Replace_Blank(uint64_t StartPos, uint64_t numElements, uint64_t newNumElements)
{
	int64_t num = newNumElements - numElements;

	if (num > 0) {
		Reserve(num);
		return Insert_Blank(num, StartPos);
	}
	else if (num < 0) {
		return Remove(num, StartPos);
	}

	return begin(StartPos);
}

inline void iBlockD::Replace(const iBlock* block, uint64_t StartPos, uint64_t numElements)
{
	memmove(Replace_Blank(StartPos,numElements, block->Size()), block->begin(), block->Size());
}





