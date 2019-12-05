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

template <size_t SIZE>
class TableStruct : public iTableFlexible {
public:
	TableStruct() {};
	TableStruct(iDBlock* parentBlock);
	inline void InitializeTable();
	size_t Size() const override { return SIZE; };
	iDBlock* ParentBlock() override { return _ParentBlock; };
	const iDBlock* ParentBlock() const override { return _ParentBlock; };
	const size_t* Index() const override { return (size_t*)index; };
	size_t* Index() override { return (size_t*)index; };
	virtual uint8_t* begin(const int64_t& offset = 0) override { return ParentBlock()->begin()+offset; };
	virtual const uint8_t* begin(const int64_t& offset = 0) const  override { return ParentBlock()->begin() + offset; };
	virtual uint8_t* end(const int64_t& offset = 0) override { return begin(Index()[SIZE]); };
	virtual const uint8_t* end(const int64_t& offset = 0) const override { return begin(Index()[SIZE]); };
	virtual Partition GetPartition(const size_t& index) override;
private:
	iDBlock* _ParentBlock = 0;
	size_t* index[SIZE+1];//last index is end of table
};


template <size_t SIZE>
TableStruct<SIZE>::TableStruct(iDBlock* parentBlock) : _ParentBlock(parentBlock)
{
	InitializeTable();
}

template <size_t SIZE>
inline void TableStruct<SIZE>::InitializeTable()
{
	for (size_t i = 0; i < SIZE+1; i++)
		Index()[i] = i;
}

template <size_t SIZE>
inline Partition TableStruct<SIZE>::GetPartition(const size_t& index)
{
	return Partition(this, index);
}