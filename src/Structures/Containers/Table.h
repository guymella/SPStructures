#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Block
    @ingroup Block
    @brief inline Block of dynamic Memory, Self Allocating on heap
*/

#include "Structures/Types.h"
#include "Structures/Interfaces/iTable.h"
#include "Structures/Containers/Partition.h"
#include "Array.h"

class Table : public iTableEditable {
public:
	Table() {};
	Table(const size_t& size);
	size_t Size() const override { return index.Size()-1; };
	iDBlock* ParentBlock() override { return &block; };
	const iDBlock* ParentBlock() const override { return &block; };
	Partition GetPartition(const size_t& index) override;
	//size_t GetPartitionSize(const size_t& index) const override;
	/*void GrowPartition(const size_t& index) override;
	void GrowPartition(const size_t& index, const size_t& NewSize) override;
	void GrowPartition(const size_t& index, const size_t& NewSize, const size_t& frontPorch) override;*/

	//void* FindPartitionMem(size_t index) override;
	
	//size_t IndexSpace() const;
	//size_t UsedSpace() const;
	//size_t FreeSpace() const;
	//const size_t* Index() const;
	//size_t* Index();
	virtual iDArray<size_t>& GetIndex() override { return index; };
	virtual const iDArray<size_t>& GetIndex() const override { return index; };
	virtual Partition Push(size_t NewSize = 0) override;//add new partition to end of table

private:
	
	Array<size_t> index;
	DBlock block;
	//dynamic size is stored inline at beggining of memory block

};


Table::Table( const size_t& size) 
{
	for (size_t i = 0; i < size+1; i++)
		index.PushBack(0);
}

inline Partition Table::GetPartition(const size_t& index) // do not use
{
	return Partition(this,index);
	
}



//inline void Table::GrowPartition(const size_t& index)
//{
//	size_t s = Size();
//	if (s)
//		GrowPartition(index,(size_t)((double)s * 1.618));
//	else
//		GrowPartition(index,9);
//}
//
//inline void Table::GrowPartition(const size_t& index, const size_t& NewSize)
//{
//	//if (index >= Size()) return;
//
//	if (NewSize <= Index()[index]) return;
//
//	size_t needed = NewSize - Index()[index];
//	int64_t growNeeded = needed - FreeSpace();
//
//
//	//TODO:: replace with copy map grow
//	if (growNeeded > 0)
//		ParentBlock()->Grow(ParentBlock()->Size() + needed);
//
//	//shift fallowing partitions
//	if (index < Size() - 1) {
//		void* cb = FindPartitionMem(index + 1);
//		size_t cs = (size_t)MemEnd() - (size_t)cb;
//		void* cd = (uint8_t*)cb + needed;
//		/*int* cur = (int*)cb;
//		int x = cur[0], y = cur[1];*/
//		memmove(cd, cb, cs);
//
//		/*int* aft = (int*)cd;
//		x = aft[0], y = aft[1];
//		x = 1;*/
//	}
//	
//
//	//increase desired partiton size
//	Index()[index] = NewSize;
//
//
//}
//
//inline void Table::GrowPartition(const size_t& index, const size_t& NewSize, const size_t& frontPorch)
//{
//	//todo::
//}
//
//inline void* Table::FindPartitionMem(size_t index)
//{
//	if (!Size() || index >= Size())
//		return NULL;
//	
//	uint8_t* b = (uint8_t*)(begin());
//	
//	size_t* partIndex = Index();
//	//b is index of first block
//	for (size_t i = 0; i < index;  i++) {
//		b += partIndex[i];
//	}
//
//	return b;
//}

inline Partition Table::Push(size_t NewSize)
{
	GetIndex().PushBack(GetIndex().Back());
	GrowPartition(Size() - 1, NewSize);
	return Partition(this,Size()-1);
}

