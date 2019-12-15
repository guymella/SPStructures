#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Partition
    @ingroup Block
    @brief inline Block of dynamic Memory, knows its place in parent table for dipendancy inversion.
*/

#include "Structures/Types.h"
#include "Structures/Interfaces/iBlock.h"
#include "Structures/Interfaces/iTable.h"
#include <string.h>
#include <memory>

//class iTableFlexible;

class Partition : public iDBlock { 
public:
	Partition(iTableFlexible* parentTable, size_t index);
	
	virtual size_t Capacity() const override;
	size_t Size() const override;
	void Grow() override;
	void Grow(const size_t& newSize) override;
	void Grow(const size_t& newSize, const size_t& frontPorch) override;
	void GrowMap(const CopyMap& map) override;
	void * memStart() override;
	const void* memStart() const override;

private:
	iTableFlexible* ParentTable = 0;
	size_t TableIndex;
	//dynamic size is stored inline at beggining of memory block

};



Partition::Partition(iTableFlexible* parentTable, size_t index) : ParentTable(parentTable), TableIndex(index)
{

}

inline size_t Partition::Capacity() const
{
	return ParentTable->GetPartitionCapacity(TableIndex);
}

inline size_t Partition::Size() const
{
	return ParentTable->GetPartitionSize(TableIndex);
}

inline void Partition::Grow()
{
	ParentTable->GrowPartition(TableIndex);
}

inline void Partition::Grow(const size_t& newSize)
{
	ParentTable->GrowPartition(TableIndex, newSize);
}

inline void Partition::Grow(const size_t& newSize, const size_t& frontPorch)
{
	ParentTable->GrowPartition(TableIndex, newSize, frontPorch);
}

inline void Partition::GrowMap(const CopyMap& map)
{
	//TODO::
}

inline void* Partition::memStart()
{
	return ParentTable->FindPartitionMem(TableIndex);
}

inline const void* Partition::memStart() const
{
	return ParentTable->FindPartitionMem(TableIndex);
}


