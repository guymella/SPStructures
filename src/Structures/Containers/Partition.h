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
	void * memStart() override;
	const void* memStart() const override;

private:
	iTable* ParentTable = 0;
	size_t Index;
	//dynamic size is stored inline at beggining of memory block

};



Partition::Partition(iTable* parentTable, size_t index) : ParentTable(parentTable), Index(index)
{

}

inline size_t Partition::Size() const
{
	return ParentTable->GetPartitionSize(Index);
}

inline void Partition::Grow()
{
	ParentTable->GrowPartition(Index);
}

inline void Partition::Grow(const size_t& newSize)
{
	ParentTable->GrowPartition(Index, newSize);
}

inline void* Partition::memStart()
{
	return ParentTable->FindPartitionMem(Index);
}

inline const void* Partition::memStart() const
{
	return ParentTable->FindPartitionMem(Index);
}


