#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iTable
    @ingroup Interfaces
    @brief interface for Procedural inline Memory Table. subdivides a block into multiple dynamic partitions.
*/
//#include <stdint.h>
#include "Structures/Types.h"
#include "iCountable.h"
#include "iIterable.h"
//#include "iArray.h"
//#include "Structures/Containers/Partition.h"

//#include "Structures/Containers/Partition.h"

//class Partition;
class iBlock;

template<typename TYPE = iBlock>
class iTable : public iCountable, public iIterable<TYPE>
{
public:
	//Size of table
	virtual size_t Size() const override = 0;
	/// C++ conform begin
	virtual Itr<TYPE> begin(const int64_t& offset = 0) override = 0;
	/// C++ conform begin
	virtual Itr<const TYPE> begin(const int64_t& offset = 0) const override = 0;
	/// C++ conform end
	virtual Itr<TYPE> end(const int64_t& offset = 0) override = 0;
	/// C++ conform end
	virtual Itr<const TYPE> end(const int64_t& offset = 0) const override = 0;

	//size_t* GetCopyMap() const; //returns a copy map tree
	//virtual iBlock* ParentBlock() = 0;
	//virtual const iBlock* ParentBlock() const = 0;		
	//virtual size_t* Index() = 0;
	//virtual const size_t* Index() const = 0;
	//virtual Partition GetPartition(const size_t& index)=0; //returns partition at index as block
	//virtual size_t GetPartitionSize(const size_t& index) const;
	//virtual size_t GetPartitionCapacity(const size_t& index) const;
	//virtual void* FindPartitionMem(size_t index);
		
};
 //

	//class iTableFlexible : public iTable
	//{
	//public:
	//	virtual iDBlock* ParentBlock() = 0;
	//	virtual const iDBlock* ParentBlock() const = 0;	
	//	//virtual Partition GetPartition(const size_t& index) override;
	//	virtual void GrowPartition(const size_t& index); // dumb grow partition
	//	virtual void GrowPartition(const size_t& index, const size_t& NewSize); // grow partition to specific size
	//	virtual void GrowPartition(const size_t& index, const size_t& NewSize, const size_t& frontPorch); // grow partition to specific size
	//	virtual size_t UsedSpace() const;
	//	virtual size_t FreeSpace() const;
	//																										  //void GrowPartition(const size_t& index, const size_t& NewSize, size_t* CopyMap); // grow partition to specific size with copy map
	//		/// C++ conform begin
	//	virtual Itr<uint8_t> begin(const int64_t& offset = 0) { return ParentBlock()->begin(offset); };
	//	/// C++ conform begin
	//	virtual Itr<const uint8_t> begin(const int64_t& offset = 0) const { return ParentBlock()->begin(offset); };
	//	/// C++ conform end
	//	virtual Itr<uint8_t> end(const int64_t& offset = 0) { return ParentBlock()->begin(UsedSpace() + offset); };
	//	/// C++ conform end
	//	virtual Itr<const uint8_t> end(const int64_t& offset = 0) const { return ParentBlock()->begin(UsedSpace() + offset); };
	//};

	//class iTableEditable : public iTableFlexible
	//{
	//public:
	//	virtual iDArray<size_t>& GetIndex() = 0;
	//	virtual const iDArray<size_t>& GetIndex() const = 0;
	//	virtual Partition Push(size_t NewSize = 0) = 0;//add new partition to end of table
	//	//virtual iBlock* Insert(const size_t& index)=0;
	//	//virtual iBlock* Insert(const size_t& index, const size_t& NewSize)=0;
	//	//virtual iBlock* Insert(const size_t& index, const iBlock& ToCopy)=0;

	//	virtual size_t* Index() override { return GetIndex().begin(); };
	//	virtual const size_t* Index() const { return GetIndex().begin(); };
	//	//virtual size_t FreeSpace() const override;
	//};



	////inline Partition iTableFlexible::GetPartition(const size_t& index)
	////{
	////	if (index >= Size())
	////		return Partition(this, Size()-1);
	////	return Partition(this,index);
	////}

	//inline size_t iTable::GetPartitionSize(const size_t& index) const
	//{
	//	if (index >= Size())
	//		return 0;

	//	return Index()[index + 1] - Index()[index];
	//}

	//inline size_t iTable::GetPartitionCapacity(const size_t& index) const
	//{
	//	if (index >= Size())
	//		return 0;

	//	return Index()[index + 1] - Index()[index];
	//}

	//inline void* iTable::FindPartitionMem(size_t index)
	//{
	//	if (!Size() || index >= Size())
	//		return NULL;

	//	return begin(Index()[index]);
	//}




	//inline void iTableFlexible::GrowPartition(const size_t& index)
	//{
	//	size_t s = Size();
	//	if (s)
	//		GrowPartition(index, (size_t)((double)s * 1.618));
	//	else
	//		GrowPartition(index, 9);
	//}

	//inline void iTableFlexible::GrowPartition(const size_t& index, const size_t& NewSize)
	//{
	//	//if (index >= Size()) return;

	//	if (NewSize <= GetPartitionSize(index)) return;

	//	size_t needed = NewSize - GetPartitionSize(index);
	//	int64_t growNeeded = needed - FreeSpace();


	//	//TODO:: replace with copy map grow
	//	if (growNeeded > 0)
	//		ParentBlock()->Grow(ParentBlock()->Size() + needed);

	//	//shift fallowing partitions
	//	if (index < Size() - 1) {
	//		void* cb = FindPartitionMem(index + 1);
	//		size_t cs = (size_t)end() - (size_t)cb;
	//		void* cd = (uint8_t*)cb + needed;
	//		/*int* cur = (int*)cb;
	//		int x = cur[0], y = cur[1];*/
	//		memmove(cd, cb, cs);

	//		/*int* aft = (int*)cd;
	//		x = aft[0], y = aft[1];
	//		x = 1;*/
	//	}


	//	//increase desired partiton size
	//	for (size_t i = index+1; i < Size()+1; i++)
	//		Index()[i] += NewSize;


	//}

	//inline void iTableFlexible::GrowPartition(const size_t& index, const size_t& NewSize, const size_t& frontPorch)
	//{
	//	//todo::
	//}
	//inline size_t iTableFlexible::UsedSpace() const
	//{
	//	return Index()[Size()];
	//}

	//inline size_t iTableFlexible::FreeSpace() const
	//{
	//	return ParentBlock()->Size() - UsedSpace();
	//}

	////Partition iTableEditable::Push(size_t NewSize)
	////{
	////	GetIndex().PushBack(GetIndex().Back());
	////	GrowPartition(Size() - 1, NewSize);
	////	return Partition(this,Size()-1);
	////}