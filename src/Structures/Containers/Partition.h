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

class iTable;

class Partition : public iDBlock { 
public:
	Partition(iTable* parentTable, size_t index);
	
	
	size_t Size() const override;
	void Grow() override;
	void Grow(const size_t& newSize) override;
	void Grow(const size_t& newSize, const size_t& frontPorch) override;
	void GrowCopyMap(const size_t& newSize, CopyRange* CopyMap, size_t CopyMapSize) override;
	void * memStart() override;
	const void* memStart() const override;

private:
	iTable* ParentTable = 0;
	size_t TableIndex;
	//dynamic size is stored inline at beggining of memory block

};



Partition::Partition(iTable* parentTable, size_t index) : ParentTable(parentTable), TableIndex(index)
{

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

inline void Partition::GrowCopyMap(const size_t& newSize, CopyRange* CopyMap, size_t CopyMapSize)
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


