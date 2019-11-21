#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iTable
    @ingroup Interfaces
    @brief interface for Procedural inline Memory Table. subdivides a block into multiple dynamic partitions.
*/
#include <stdint.h>
#include "iBlock.h"
//#include "Structures/Containers/Partition.h"

	class iTable{
	public:
		//size_t* GetCopyMap() const; //returns a copy map tree
		virtual iBlock* ParentBlock() = 0;
		virtual const iBlock* ParentBlock() const = 0;
		virtual size_t Size() const = 0; //number of partitions		
		virtual iBlock* GetPartition(const size_t& index) = 0; //returns partition at index as block
		virtual size_t GetPartitionSize(const size_t& index) const = 0;
		virtual void GrowPartition(const size_t& index) =0; // dumb grow partition
		virtual void GrowPartition(const size_t& index, const size_t& NewSize) =0; // grow partition to specific size
		//void GrowPartition(const size_t& index, const size_t& NewSize, size_t* CopyMap); // grow partition to specific size with copy map
		//iBlock* Push();  //add new partition to end of table
		//iBlock* Push(const size_t& NewSize);
		//iBlock* Insert(const size_t& index);
		//iBlock* Insert(const size_t& index, const size_t& NewSize);
		//iBlock* Insert(const size_t& index, const iBlock& ToCopy);

		virtual void* FindPartitionMem(size_t index) = 0;

		//void Free(); //deallocate table and 
	};
