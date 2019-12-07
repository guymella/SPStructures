#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Block
    @ingroup Block
    @brief inline Block of dynamic Memory, Self Allocating on heap
*/

#include "Structures/Types.h"
#include "Structures/Interfaces/iBlock.h"
#include <string.h>
#include <memory>



template <size_t SIZE>
class Block : public iBlock {
public:
	Block();
	
	size_t Size() const override;
	void* memStart() override;
	const void* memStart() const override;
	/// copy-assignment operator (truncates to actual size)
	void operator=(const Block& rhs) {copy(rhs);};

	//void FastCopy(uint8_t* src, size_t StartIndex, size_t numBytes); //copy bytes into block
	//void FastCopy(DBlock& src, size_t srcStartIndex, size_t StartIndex, size_t numBytes); //copy block into block
private:
	uint8_t block[SIZE];
	//dynamic size is stored inline at beggining of memory block
	void copy(const Block& rhs) override;
};

class DBlock : public iDBlock {
public:
	DBlock();
	~DBlock();
	
	inline void InitializeBlock(const size_t& size);
	DBlock(const size_t& size);
	/// copy-assignment operator (truncates to actual size)
	void operator=(const DBlock& rhs) { copy(rhs); };
	/// move-assignment operator (same capacity and size)
	void operator=(DBlock&& rhs) { move(std::move(rhs)); };
	
	bool Initialized() const;
	size_t Size() const override;
	void Grow() override;
	void Grow(const size_t& newSize) override;
	void Grow(const size_t& newSize, const size_t& frontPorch) override;
	void GrowCopyMap(const size_t& newSize, CopyRange* CopyMap, size_t CopyMapSize) override;
	void * memStart() override;
	const void* memStart() const override;


	//void FastCopy(uint8_t* src, size_t StartIndex, size_t numBytes); //copy bytes into block
	//void FastCopy(DBlock& src, size_t srcStartIndex, size_t StartIndex, size_t numBytes); //copy block into block
private:
	void* blockStart = 0;
	size_t size = 0;
	void copy(const DBlock& rhs);
	void move(DBlock&& rhs);
	void destroy();
	//dynamic size is stored inline at beggining of memory block

};

DBlock::DBlock() {
};
DBlock::~DBlock()
{
	destroy();
}

void DBlock::destroy()
{
	if (blockStart)
		free(blockStart);
}

inline void DBlock::InitializeBlock(const size_t& newSize)
{
	size = newSize;
	blockStart = newSize ? std::malloc(size) : 0;
	//std::memset(blockStart, 0, size);
}

DBlock::DBlock(const size_t& size)  {
	InitializeBlock(size);
}

inline size_t DBlock::Size() const
{
	return size;
}

inline bool DBlock::Initialized() const
{
	return blockStart;
}

inline void DBlock::Grow()
{
	size_t s = Size();
	if (s)
		Grow((size_t)((double)s * 1.618));
	else
		Grow(9);
	

}

inline void DBlock::Grow(const size_t& newSize)
{ 
	Grow(newSize, 0);
}
inline void DBlock::Grow(const size_t& newSize, const size_t& frontPorch)
{
	size_t oldSize = Size();
	/*if (newSize > oldSize)
	{*/
		void* oldBlock = blockStart;
		void* oldStart = begin();
		InitializeBlock(newSize);
		if (oldBlock) {// = 0 if was not initialized
			void* newStart = begin(frontPorch);
			memcpy(newStart, oldStart, oldSize);
			free(oldBlock);
		}
	//}
}
inline void DBlock::GrowCopyMap(const size_t& newSize, CopyRange* CopyMap, size_t CopyMapSize)
{
	void* oldBlock = blockStart;
	InitializeBlock(newSize);
	
	for (size_t i = 0; i < CopyMapSize; i++) {
		memcpy(begin()+CopyMap[i].dstOffset, CopyMap[i].src, CopyMap[i].size);
	}
	free(oldBlock);
}
inline void* DBlock::memStart()
{
	return blockStart;
}
inline const void* DBlock::memStart() const
{
	return blockStart;
}
inline void DBlock::copy(const DBlock& rhs)
{
	destroy();
	if (rhs.size) {
		InitializeBlock(rhs.size);
		memcpy(memStart(), rhs.memStart(), size);
	}
	
}
inline void DBlock::move(DBlock&& rhs)
{
	size = rhs.size;
	blockStart = rhs.blockStart;
	rhs.blockStart = 0;
	rhs.size = 0;
}
;

template<size_t SIZE>
inline size_t Block<SIZE>::Size() const
{
	return SIZE;
}

template<size_t SIZE>
inline void* Block<SIZE>::memStart()
{
	return (block& + offset);
}

template<size_t SIZE>
inline const void* Block<SIZE>::memStart() const
{
	return (block& + offset);
}

template<size_t SIZE>
inline void Block<SIZE>::copy(const Block& rhs)
{
	size_t s = rhs.Size();
	if (s > Size())
		s = Size();

	memcpy(memStart(), rhs.memStart(), s);
}
