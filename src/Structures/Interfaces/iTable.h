#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iTable
    @ingroup Interfaces
    @brief interface for Procedural inline Memory Table. subdivides a block into multiple dynamic partitions.
*/
#include <stdint.h>
#include "iBlock.h"

	class iTable{
	public:
		iTable(iBlock* ParentBlock);
		iTable(iBlock* ParentBlock, iTable* SourceTable);

		size_t* GetCopyMap() const; //returns a copy map tree
		iBlock GetPartition(const size_t& index); //returns partition at index as block
		size_t Size() const; //number of partitions

		//void Free(); //deallocate table and 
	private:
		void* memStart;
		iBlock* ParentBlock; //must exist inside a block
	};
