#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Block
    @ingroup Block
    @brief inline Block of dynamic Memory, Self Allocating on heap
*/


#include "Interfaces/iBlock.h"
//#include <string.h>
//#include <memory>

class Block : public iBlock {
public:
	//Size of table
	virtual size_t Size() const override = 0;

protected:
	//first address of blocks memory
	virtual inline uint8_t* memStart() = 0;
	//first address of blocks memory
	virtual inline const uint8_t* memStart() const = 0;
};




template <size_t SIZE>
class BlockS : public Block 
{
public:
	BlockS() {};
	//CopyConstructor
	BlockS(const iBlock& rhs) { Copy(rhs); };
	//Size of Block
	virtual size_t Size() const override { return SIZE; };
	
protected:
	//first address of blocks memory
	virtual inline uint8_t* memStart() override { return block; };
	//first address of blocks memory
	virtual inline const uint8_t* memStart() const override { return block; };
	
private:
	uint8_t block[SIZE];
	
};



class BlockD : public iBlockD {
public:
	//Size of table
	virtual size_t Size() const override = 0;
	// remove all spare reallocate to smallest cappacity that fits current size
	virtual void Trim() override = 0;
	//Reiize to the size Specified (trancates if smaller)
	virtual void Resize(const size_t& newSize) override = 0;
	/// total number of additional aelements that can be added to front without reallocation
	virtual size_t SpareFront() const override = 0;
	/// realocate and include At least numelelents Spare at front
	virtual void ReserveFront(const size_t& numElements) override = 0;
	/// total number of additional elements that can be added to back without reallocation
	virtual size_t SpareBack() const override = 0;
	/// realocat and include at least numElements Spare At Back
	virtual void ReserveBack(const size_t& numElements) override = 0;
protected:
	//first address of blocks memory
	virtual inline uint8_t* memStart()  override = 0;
	//first address of blocks memory
	virtual inline const uint8_t* memStart() const override = 0;


	/*void* blockStart = 0;
	size_t size = 0;
	void copy(const DBlock& rhs);
	void move(DBlock&& rhs);
	void destroy();*/
	//dynamic size is stored inline at beggining of memory block

};


//Dynamic block always full
class BlockDP : public BlockD
{
public:
	BlockDP() {};
	BlockDP(const size_t& size) { AllocateBlock(size); };
	//CopyConstructor
	BlockDP(const iBlock& rhs) { Copy(rhs); };
	//CopyConstructor
	BlockDP(const BlockDP& rhs) { Copy(rhs); };
	//move Constructor
	BlockDP(BlockDP&& rhs) { Move(std::move(rhs)); };
	//destructor
	~BlockDP() { if (blockPtr) std::free(blockPtr); }
	/// move-assignment operator (same capacity and size)
	void operator=(BlockDP&& rhs) noexcept { Move(std::move(rhs)); };
	//Size of Block
	virtual inline size_t Size() const override { return size; };

	// remove all spare reallocate to smallest cappacity that fits current size
	virtual void Trim() override { return; } //always trimmed
	/// total number of additional aelements that can be added to front without reallocation
	virtual size_t SpareFront() const override { return 0; };
	/// realocate and include At least numelelents Spare at front
	virtual void ReserveFront(const size_t& numElements) override;
	/// total number of additional elements that can be added to back without reallocation
	virtual size_t SpareBack() const override { return 0; };
	/// realocat and include at least numElements Spare At Back
	virtual void ReserveBack(const size_t& numElements) override;
	//Reiize to the size Specified (trancates if smaller)
	virtual void Resize(const size_t& newSize) override;
protected:
	//Copy From Another Block
	virtual void Copy(const iBlock& rhs) override;
	//Move From Another Block
	virtual void Move(BlockDP&& rhs);
	//first address of blocks memory
	virtual inline uint8_t* memStart() override{ return blockPtr; };
	//first address of blocks memory
	virtual inline const uint8_t* memStart() const override { return blockPtr; };
	void ReAllocateBlock(const size_t& size);
	uint8_t* AllocateBlock(const size_t& size);
	size_t size = 0;
	uint8_t* blockPtr = 0;
};

class BlockDV : public BlockDP
{
public:
	BlockDV() {};
	BlockDV(const size_t& size);
	//CopyConstructor
	BlockDV(const iBlock& rhs) { Copy(rhs); };
	//CopyConstructor
	BlockDV(const BlockDV& rhs) { Copy(rhs); };
	//move Constructor
	BlockDV(BlockDV&& rhs) { Move(std::move(rhs)); };
	/// move-assignment operator (same capacity and size)
	void operator=(BlockDV&& rhs) noexcept { Move(std::move(rhs)); };
	//destructor
	//~BlockDV() { if (blockPtr) std::free(blockPtr); };
	//Total Capacity of container without reallocation
	virtual size_t Capacity() const override { return capacity; };
	// remove all spare reallocate to smallest cappacity that fits current size
	virtual void Trim() override;
	/// realocate and include At least numelelents Spare at front
	virtual void ReserveFront(const size_t& numElements) override;
	/// total number of additional elements that can be added to back without reallocation
	virtual size_t SpareBack() const override { return Capacity() - Size(); };
	/// realocat and include at least numElements Spare At Back
	virtual void ReserveBack(const size_t& numElements) override;
	//Reiize to the size Specified (trancates if smaller)
	virtual void Resize(const size_t& newSize) override;
protected:
	//Move From Another Block
	virtual void Move(BlockDV&& rhs); 

	void ReAllocateBlock(const size_t& size);
	uint8_t* AllocateBlock(const size_t& size);

	size_t capacity = 0;
private:
};


inline void BlockDP::ReserveFront(const size_t& numElements)
{
	size_t oldSize = size;
	uint8_t* oldBlock = AllocateBlock(size + numElements);
	if (oldBlock) {
		std::memcpy(blockPtr + numElements, oldBlock, oldSize);
		std::free(oldBlock);
	}
}

inline void BlockDP::ReserveBack(const size_t& sizeAdd)
{
	size_t oldSize = size;
	uint8_t* oldBlock = AllocateBlock(size+sizeAdd);
	if (oldBlock) {
		std::memcpy(blockPtr, oldBlock, oldSize);
		std::free(oldBlock);
	}
}

inline void BlockDP::Resize(const size_t& newSize)
{
	size_t oldSize = size;
	uint8_t* oldBlock = AllocateBlock(newSize);
	if (oldBlock) {
		std::memcpy(blockPtr, oldBlock,((oldSize<newSize)? oldSize : newSize));
		std::free(oldBlock);
	}
}

void BlockDP::Copy(const iBlock& rhs)
{
	ReAllocateBlock(rhs.Size());
	iBlock::Copy(rhs);
}

void BlockDP::Move(BlockDP&& rhs)
{
	size = rhs.size;
	blockPtr = rhs.blockPtr;
	rhs.size = 0;
	rhs.blockPtr = 0;
}

void BlockDP::ReAllocateBlock(const size_t& newSize)
{
	std::free(AllocateBlock(newSize));
}

uint8_t* BlockDP::AllocateBlock(const size_t& newSize)
{
	uint8_t* oldBlockPtr = blockPtr;
	size = newSize;
	blockPtr = (uint8_t*)std::malloc(size);
	return oldBlockPtr;
}

inline BlockDV::BlockDV(const size_t& newSize) 
{
	AllocateBlock(newSize);
	size = newSize;
}

inline void BlockDV::Trim()
{
	if (Size() < Capacity()) {
		if (Size()) {
			uint8_t* oldBlock = AllocateBlock(size);
			if (oldBlock) {
				std::memcpy(blockPtr, oldBlock, size);
				std::free(oldBlock);
			}
		}
		else {
			std::free(blockPtr);
			capacity = 0;
		}		
	}	
}

inline void BlockDV::ReserveFront(const size_t& numElements)
{
	BlockDP::ReserveFront(numElements);
	capacity += numElements;
}

inline void BlockDV::ReserveBack(const size_t& sizeAdd)
{
	if (SpareBack() >= sizeAdd) {
		size += sizeAdd;
		return;
	}

	size_t oldSize = size;
	size += sizeAdd;
	uint8_t* oldBlock = AllocateBlock(size);
	if (oldBlock) {
		std::memcpy(blockPtr, oldBlock, oldSize);
		std::free(oldBlock);
	}
}

inline void BlockDV::Resize(const size_t& newSize)
{
	if (Size() + Spare() >= newSize) {
		size = newSize;
		return;
	}

	size_t oldSize = size;
	size = newSize;
	uint8_t* oldBlock = AllocateBlock(newSize);
	if (oldBlock) {
		std::memcpy(blockPtr, oldBlock, ((oldSize < newSize) ? oldSize : newSize));
		std::free(oldBlock);
	}
}

void BlockDV::Move(BlockDV&& rhs)
{
	capacity = rhs.capacity;
	size = rhs.size;
	blockPtr = rhs.blockPtr;
	rhs.size = 0;
	rhs.capacity = 0;
	rhs.blockPtr = 0;
}

inline void BlockDV::ReAllocateBlock(const size_t& newCapacity)
{
	uint8_t* oldBlock = AllocateBlock(newCapacity);
	if (oldBlock)
		std::free(oldBlock);
	size = 0;
}

inline uint8_t* BlockDV::AllocateBlock(const size_t& newCapacity)
{
	uint8_t* oldBlockPtr = blockPtr;
	capacity = newCapacity;
	blockPtr = (uint8_t*)std::malloc(capacity);
	return oldBlockPtr;
}

//
//class DBlock : public iDBlock {
//public:
//	DBlock();
//	DBlock(const size_t& size);
//	DBlock(const DBlock& rhs);
//	DBlock(DBlock&& rhs);
//	~DBlock();
//	
//	inline void InitializeBlock(const size_t& size);
//	inline bool Initialized() const { return blockStart; };	
//	/// move-assignment operator (same capacity and size)
//	void operator=(const DBlock& rhs) { copy(rhs); };
//	/// move-assignment operator (same capacity and size)
//	void operator=(DBlock&& rhs) noexcept { move(std::move(rhs)); };
//	
//	virtual void GrowMap(const CopyMap& map) override;
//	virtual size_t Capacity() const override { return size; };
//	virtual inline size_t Size() const override { return size; };
//	virtual inline void* memStart() override { return blockStart; };
//	virtual inline const void* memStart() const override { return blockStart; };
//private:
//	void* blockStart = 0;
//	size_t size = 0;
//	void copy(const DBlock& rhs);
//	void move(DBlock&& rhs);
//	void destroy();
//	//dynamic size is stored inline at beggining of memory block
//
//};
//
//DBlock::DBlock() {
//};
//DBlock::~DBlock()
//{
//	destroy();
//}
//
//void DBlock::destroy()
//{
//	if (blockStart)
//		free(blockStart);
//	size = 0;
//}
//
//inline void DBlock::InitializeBlock(const size_t& newSize)
//{
//	size = newSize;
//	blockStart = newSize ? std::malloc(size) : 0;
//}
//
//DBlock::DBlock(const size_t& size)  {
//	InitializeBlock(size);
//}
//
//inline void DBlock::GrowMap(const CopyMap& map)
//{
//	void* oldBlock = memStart();
//	InitializeBlock(map.newSize);
//	if (oldBlock) {
//		map.CopyTo((uint8_t*)memStart());
//		free(oldBlock);
//	}
//}
//
//
//
//inline void DBlock::copy(const DBlock& rhs)
//{
//	destroy();
//	if (rhs.size) {
//		InitializeBlock(rhs.size);
//		memcpy(memStart(), rhs.memStart(), size);
//	}	
//}
//inline void DBlock::move(DBlock&& rhs)
//{
//	size = rhs.size;
//	blockStart = rhs.blockStart;
//	rhs.blockStart = 0;
//	rhs.size = 0;
//};
//
//

