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

class Table : public iTable {
public:
	Table() {};
	Table(iDBlock* parentBlock);
	Table(iDBlock* parentBlock, const size_t& size);
	inline void InitializeTable(const size_t& size);
	bool Initialized() const;
	size_t Size() const override;
	iDBlock* ParentBlock() override;
	const iDBlock* ParentBlock() const override;
	iDBlock* GetPartition(const size_t& index) override;
	size_t GetPartitionSize(const size_t& index) const override;
	void GrowPartition(const size_t& index) override;
	void GrowPartition(const size_t& index, const size_t& NewSize) override;
	void GrowPartition(const size_t& index, const size_t& NewSize, const size_t& frontPorch) override;

	void* FindPartitionMem(size_t index) override;
	void* MemStart();
	const void* MemStart() const;
	void* MemEnd();
	const void* MemEnd() const;
	size_t IndexSpace() const;
	size_t UsedSpace() const;
	size_t FreeSpace() const;
	const size_t* Index() const;
	size_t* Index();

private:
	

	iDBlock* _ParentBlock = 0;
	//dynamic size is stored inline at beggining of memory block

};



Table::Table(iDBlock* parentBlock) : _ParentBlock(parentBlock)
{
	InitializeTable(0);
}

Table::Table(iDBlock* parentBlock, const size_t& size) : _ParentBlock(parentBlock)
{
	InitializeTable(size);
}

inline void Table::InitializeTable(const size_t& size)
{//set contents of block to table and init
	size_t headerSize = sizeof(baseTypes) + sizeof(size_t) * (size + 1)+ size;
	if (ParentBlock()->MemSize() < headerSize) {
		ParentBlock()->Grow(headerSize);
	}
	//set type
	ParentBlock()->SetType(baseTypes::MemTable);
	//set header
	size_t* s = (size_t*)ParentBlock()->headerStart();
	s[0] = size;
	baseTypes* m = (baseTypes*)MemStart();
	for (size_t i = 0; i < size; i++) {
		s[i+1] = 1;
		m[i] = baseTypes::Void;
	}
}

bool Table::Initialized() const
{
	if (ParentBlock() && ParentBlock()->MemSize()) {
		return ParentBlock()->Type() == baseTypes::MemTable;
	}
	return false;
}

size_t Table::Size() const
{
	if (Initialized())
		return *((size_t*)(ParentBlock()->headerStart()));

	return 0;
}

inline iDBlock* Table::ParentBlock()
{
	return _ParentBlock;
}

inline const iDBlock* Table::ParentBlock() const
{
	return _ParentBlock;
}

inline iDBlock* Table::GetPartition(const size_t& index) // do not use
{
	return new Partition(this,index);
	
}

inline size_t Table::GetPartitionSize(const size_t& index) const
{
	if (index >= Size())
		return 0;

	return Index()[index];
}

inline void Table::GrowPartition(const size_t& index)
{
	size_t s = Size();
	if (s)
		GrowPartition(index,(size_t)((double)s * 1.618));
	else
		GrowPartition(index,9);
}

inline void Table::GrowPartition(const size_t& index, const size_t& NewSize)
{
	//if (index >= Size()) return;

	if (NewSize <= Index()[index]) return;

	size_t needed = NewSize - Index()[index];
	int64_t growNeeded = needed - FreeSpace();


	//TODO:: replace with copy map grow
	if (growNeeded > 0)
		ParentBlock()->Grow(ParentBlock()->MemSize() + needed);

	//shift fallowing partitions
	if (index < Size() - 1) {
		void* cb = FindPartitionMem(index + 1);
		size_t cs = (size_t)MemEnd() - (size_t)cb;
		void* cd = (uint8_t*)cb + needed;
		/*int* cur = (int*)cb;
		int x = cur[0], y = cur[1];*/
		memmove(cd, cb, cs);

		/*int* aft = (int*)cd;
		x = aft[0], y = aft[1];
		x = 1;*/
	}
	

	//increase desired partiton size
	Index()[index] = NewSize;


}

inline void Table::GrowPartition(const size_t& index, const size_t& NewSize, const size_t& frontPorch)
{
	//todo::
}

inline void* Table::FindPartitionMem(size_t index)
{
	if (!Size() || index >= Size())
		return NULL;
	
	uint8_t* b = (uint8_t*)(MemStart());
	
	size_t* partIndex = Index();
	//b is index of first block
	for (size_t i = 0; i < index;  i++) {
		b += partIndex[i];
	}

	return b;
}

inline void* Table::MemStart()
{
	uint8_t* b = (uint8_t*)ParentBlock()->headerStart();
	//size_t db = (size_t)b;
	return b + IndexSpace(); //mem starts after index
}

inline const void* Table::MemStart() const
{
	uint8_t* b = (uint8_t*)ParentBlock()->headerStart();
	//size_t db = (size_t)b;
	return b + IndexSpace(); //mem starts after index
}

inline void* Table::MemEnd()
{
	uint8_t* b = (uint8_t*)MemStart();
	size_t* ind = Index();
	for (size_t i = 0; i < Size(); i++) {
		b += ind[i];
	}
	size_t db = (size_t)b;
	return b;
}

inline const void* Table::MemEnd() const
{
	uint8_t* b = (uint8_t*)MemStart();
	//size_t db = (size_t)b;
	//b += sizeof(size_t) * (Size() + 1);//offset by in
	//db = (size_t)b;
	const size_t* ind = Index();
	for (size_t i = 0; i < Size(); i++) {
		b += ind[i];
	}
	//db = (size_t)b;
	return b;
}

inline size_t Table::IndexSpace() const
{
	return sizeof(size_t) * (Size() + 1);
}

inline size_t Table::UsedSpace() const
{
	//size_t me = (size_t)MemEnd();
	//size_t ms = (size_t)ParentBlock()->memStart();

	return ((size_t)MemEnd()) - (size_t) ParentBlock()->memStart();
}

inline size_t Table::FreeSpace() const
{
	return ParentBlock()->MemSize()-UsedSpace();
}

inline const size_t* Table::Index() const
{
	return (size_t*)ParentBlock()->headerStart()+1;
}

inline size_t* Table::Index()
{
	return (size_t*)ParentBlock()->headerStart() + 1;
}
