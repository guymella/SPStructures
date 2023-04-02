#pragma once
//------------------------------------------------------------------------------
/*
    @class Structures::Allocator
    @ingroup Block
    @brief memory allocator. allocates storage and handles fragmentation
*/

//#include <stdint.h>
//#include <string.h>
//#include <memory>
//#include "Array.h"
#include "Table.h"
#include "Block.h"



class AllocatorTable : public TableD {
public:
	AllocatorTable() {};
	AllocatorTable(size_t numBlocks);
	//Size of table
	virtual size_t Size() const override { return size; };
	//add another block to the table
	virtual void PushBack(size_t blockSize = 0) override;
	//add another block to front of table
	virtual void PushFront(size_t blockSize = 0) override;
	/// C++ conform begin
	virtual Itr<iBlockD> begin(const int64_t& offset = 0) override { return Itr<iBlockD>(blocks+offset); };
	/// C++ conform begin
	virtual Itr<const iBlockD> begin(const int64_t& offset = 0) const override { return Itr<const iBlockD>(blocks + offset); };
	/// C++ conform end
	virtual Itr<iBlockD> end(const int64_t& offset = 0) override { return Itr<iBlockD>(blocks + size + offset); };
	/// C++ conform end
	virtual Itr<const iBlockD> end(const int64_t& offset = 0) const override { return Itr<const iBlockD>(blocks + size + offset); };
private:
	void allocateblocks(size_t numBlocks);
	size_t size = 0;
	BlockDV* blocks = 0; //TODO:: replace with Array of blocks
};

AllocatorTable::AllocatorTable(size_t numBlocks)
{
	allocateblocks(numBlocks);
}

inline void AllocatorTable::allocateblocks(size_t numBlocks)
{
	blocks = (BlockDV*)std::malloc(sizeof(BlockDV) * numBlocks);
	for (size_t i = 0; i < numBlocks; i++)
		new (blocks + i) BlockDV();
	size = numBlocks;
}

inline void AllocatorTable::PushBack(size_t blockSize)
{
	//TODO:: replace with Array of blocks
	BlockDV* oldBlocks = blocks;
	size_t OldSize = size;
	allocateblocks(size+1);
	for (size_t i = 0; i < OldSize; i++)
		blocks[i] = std::move(oldBlocks[i]);

	std::free(oldBlocks);
	end(-1)[0].Resize(blockSize);
}

inline void AllocatorTable::PushFront(size_t blockSize)
{
	//TODO:: replace with Array of blocks
	BlockDV* oldBlocks = blocks;
	size_t OldSize = size;
	size++;
	blocks = new BlockDV[size];
	for (size_t i = 1; i < size; i++)
		blocks[i] = std::move(oldBlocks[i-1]);

	std::free(oldBlocks);
	begin()[0].Resize(blockSize);
}



class AllocatorBlock;

template<typename TYPE = uint8_t,typename aTYPE = AllocatorBlock>
class AllocatorBlockItr : public itr<TYPE> {
public:
	AllocatorBlockItr() {};
	AllocatorBlockItr(aTYPE* a, size_t Offset = 0);
	AllocatorBlockItr(const AllocatorBlockItr<TYPE>& a);
	virtual TYPE* Ptr() override { return allocatorBlock->Table()->begin(blockIndex)[0].begin(memIndex).Ptr(); };
	virtual const TYPE* Ptr() const override { return allocatorBlock->Table()->begin(blockIndex)[0].begin(memIndex).Ptr();  };
	virtual bool operator+=(int64_t offset) override;
	virtual bool operator-=(int64_t offset) override;
	virtual int64_t operator-(const AllocatorBlockItr& rhs);
	//virtual TYPE& operator[](size_t index) = 0;
	//virtual const TYPE& operator[](size_t index) const = 0;
	virtual itr<TYPE>* MakeCopy() const override;
private:
	
	//iBlockD& getBlock(size_t index) { return allocatorBlock->Table()->begin(blockIndex)[0]; };
	//const iBlockD& getBlock(size_t index) const { return allocatorBlock->Table()->begin(blockIndex)[0]; };
	//uint8_t* getPointer() { return getBlock(blockIndex).begin(memIndex).Ptr(); }

	size_t blockIndex = 0;
	size_t memIndex = 0;
	aTYPE* allocatorBlock = 0;
};
//
//class AllocatorBlockConstItr : public itr<const uint8_t> {
//public:
//	AllocatorBlockConstItr() {};
//	AllocatorBlockConstItr(const AllocatorBlock& a, size_t Offset = 0);
//	AllocatorBlockConstItr(const AllocatorBlockConstItr& a);
//	virtual const uint8_t* Ptr() override { return memItr.Ptr(); };
//	virtual const uint8_t* Ptr() const override { return memItr.Ptr(); };
//	virtual bool operator+=(int64_t offset) override;
//	virtual bool operator-=(int64_t offset) override;
//	virtual int64_t operator-(const AllocatorBlockConstItr& rhs) override;
//	//virtual TYPE& operator[](size_t index) = 0;
//	//virtual const TYPE& operator[](size_t index) const = 0;
//	virtual itr<const uint8_t>* MakeCopy() const override;
//potected:
//	Itr<const uint8_t> memItr;
//
//	Itr<const iBlockD> blockItr;
//	Itr<const iBlockD> blockItrBegin;
//	Itr<const iBlockD> blockItrEnd;
//};

//a table linked together to create a single iterable block
class AllocatorBlock : public iBlockD{
public:
	AllocatorBlock() {};
	AllocatorBlock(const size_t& blockSize);
	// indicates presence of subtablte Table();
	virtual bool ContainsTable() const override { return true; };
	//access subtable
	virtual iTableD<>* Table() { return &table; };
	//const access subtable
	virtual const iTableD<>* Table() const { return &table;};
	//Size of table
	virtual size_t Size() const override { return Table()->BlocksSize(); };

	virtual void Trim() override {};
	//Reiize to the size Specified (trancates if smaller)
	virtual void Resize(const size_t& newSize) override;
	/// total number of additional aelements that can be added to front without reallocation
	virtual size_t SpareFront() const override { return Table()->begin()[0].SpareFront(); };
	/// realocate and include At least numelelents Spare at front
	virtual void ReserveFront(const size_t& numElements) override;
	/// total number of additional elements that can be added to back without reallocation
	virtual size_t SpareBack() const override { return Table()->end(-1)[0].SpareBack(); };
	/// realocat and include at least numElements Spare At Back
	virtual void ReserveBack(const size_t& numElements) override;

	/// C++ conform begin
	//virtual Itr<uint8_t> begin(const int64_t& offset = 0) override { return Itr<uint8_t>(new AllocatorBlockItr<>(this,offset)); };
	/// C++ conform begin
	//virtual Itr<const uint8_t> begin(const int64_t& offset = 0) const override { return Itr<const uint8_t>(new AllocatorBlockItr<const uint8_t,const AllocatorBlock>(this,offset)); };
	/// C++ conform end
	//virtual Itr<uint8_t> end(const int64_t& offset = 0) override { return begin(Size() + offset); };
	/// C++ conform end
	//virtual Itr<const uint8_t> end(const int64_t& offset = 0) const override { return begin(Size() + offset); };
	
protected:
	
	AllocatorTable table;
	//first address of blocks memory
	virtual inline uint8_t* memStart() override { return begin().Ptr(); };
	//first address of blocks memory
	virtual inline const uint8_t* memStart() const override { return begin().Ptr(); };
	
};

inline AllocatorBlock::AllocatorBlock(const size_t& blockSize)
{
	Resize(blockSize);
}

inline void AllocatorBlock::Resize(const size_t& newSize)
{
	ReserveBack(newSize - Size());
}

void AllocatorBlock::ReserveFront(const size_t& numElements)
{
	if (!Table()->Size())
		Table()->PushBack();

	Table()->begin()[0].ReserveFront(numElements);
}

inline void AllocatorBlock::ReserveBack(const size_t& numElements)
{
	if (!Table()->Size())
		Table()->PushBack();

	Table()->end(-1)[0].ReserveFront(numElements);
}

template<typename TYPE, typename aTYPE>
inline AllocatorBlockItr<TYPE, aTYPE>::AllocatorBlockItr(aTYPE* a, size_t Offset) : allocatorBlock(a)
{

	(*this) += Offset;
}
template<typename TYPE, typename aTYPE>
inline AllocatorBlockItr<TYPE,aTYPE>::AllocatorBlockItr(const AllocatorBlockItr<TYPE>& a)
{
	memIndex = a.memIndex;
	blockIndex = a.blockIndex;
	allocatorBlock = a.allocatorBlock;
}
template<typename TYPE, typename aTYPE>
inline bool AllocatorBlockItr<TYPE, aTYPE>::operator+=(int64_t offset)
{
	if (offset < 0)
		return (*this) -= (-offset);

	size_t bi = blockIndex;

	size_t curBuff = allocatorBlock->Table()->begin(bi)[0].Size() - memIndex;
	while (curBuff < (size_t)offset) {
		bi++;		
		if (bi == allocatorBlock->Table()->Size())
			return false;
		offset -= curBuff;
		curBuff = allocatorBlock->Table()->begin(bi)[0].Size();
	} //now on correct block	
	memIndex = offset;
	blockIndex = bi;
	return true;

}
template<typename TYPE, typename aTYPE>
inline bool AllocatorBlockItr<TYPE, aTYPE>::operator-=(int64_t offset)
{
	if (offset < 0)
		return (*this) += (-offset);
	size_t bi = blockIndex;
	size_t curBuff = memIndex;
	while (curBuff < (size_t)offset) {
		if (bi == 0)
			return false;
		bi--;
		offset -= curBuff;
		curBuff = allocatorBlock->Table()->begin(bi)[0].Size();
	} //now on correct block
	memIndex = curBuff - offset;
	blockIndex = bi;
	return true;
}
template<typename TYPE, typename aTYPE>
inline int64_t AllocatorBlockItr<TYPE, aTYPE>::operator-(const AllocatorBlockItr& rhs)
{
	int64_t diff = 0;
	if (rhs.allocatorBlock != allocatorBlock)
		return std::numeric_limits<int64_t>::max();

	diff -= rhs.memIndex;
	diff += memIndex;
	size_t end = (rhs.blockIndex<blockIndex) ? blockIndex : rhs.blockIndex;
	for (size_t i = ((rhs.blockIndex < blockIndex) ? rhs.blockIndex : blockIndex); i < end; i++) {
		diff += allocatorBlock->Table()->begin()[i].Size() * (rhs.blockIndex > blockIndex) ? 1 : -1;
	}

	return diff;
}
template<typename TYPE, typename aTYPE>
inline itr<TYPE>* AllocatorBlockItr<TYPE, aTYPE>::MakeCopy() const
{
	return new AllocatorBlockItr(*this);
}


//AllocatorBlockConstItr::AllocatorBlockConstItr(const AllocatorBlock& a, size_t Offset)
//{
//	blockItr = a.Table()->begin();
//	blockItrBegin = blockItr;
//	blockItrEnd = a.Table()->end();
//	memItr = blockItr[0].begin();
//	if (Offset)
//		(*this) += Offset;
//}
//
//inline AllocatorBlockConstItr::AllocatorBlockConstItr(const AllocatorBlockConstItr& a)
//{
//	memItr = a.memItr;
//	blockItr = a.blockItr;
//	blockItrBegin = a.blockItrBegin;
//	blockItrEnd = a.blockItrEnd;
//}
//
//inline bool AllocatorBlockConstItr::operator+=(int64_t offset)
//{
//	if (offset < 0)
//		return (*this) -= (-offset);
//
//	size_t curBuff = blockItr[0].end() - memItr;
//	while (curBuff < (size_t)offset) {
//		blockItr++;
//		if (blockItr == blockItrEnd)
//			return false;
//		offset -= curBuff;
//		curBuff = blockItr[0].Size();
//	} //now on correct block
//	memItr = blockItr[0].begin(offset);
//
//	return true;
//
//}
//
//inline bool AllocatorBlockConstItr::operator-=(int64_t offset)
//{
//	if (offset < 0)
//		return (*this) += (-offset);
//
//	size_t curBuff = memItr - blockItr[0].begin();
//	while (curBuff < (size_t)offset) {
//		if (blockItr == blockItrBegin)
//			return false;
//		blockItr--;
//		offset -= curBuff;
//		curBuff = blockItr[0].Size();
//	} //now on correct block
//	memItr = blockItr[0].end(-offset);
//
//	return true;
//}
//
//inline int64_t AllocatorBlockConstItr::operator-(const AllocatorBlockConstItr& rhs)
//{
//	int64_t diff = 0;
//}
//
//inline itr<const uint8_t>* AllocatorBlockConstItr::MakeCopy() const
//{
//	return new AllocatorBlockConstItr(*this);
//}
