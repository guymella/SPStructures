#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Block
    @ingroup Block
    @brief inline Block of dynamic Memory
*/

#include <stdint.h>
#include <string.h>
#include <memory>


class Block {
public:
	Block();
	inline void InitializeBlock(const size_t& size);
	Block(size_t size);
	size_t Size() const;
	bool Initialized() const;
	void Grow();
	void Grow(size_t newSize);
	void * memStart();


	void FastCopy(uint8_t* src, size_t StartIndex, size_t numBytes); //copy bytes into block
	void FastCopy(Block& src, size_t srcStartIndex, size_t StartIndex, size_t numBytes); //copy block into block
private:
	void* blockStart = 0;
	//dynamic size is stored at beggining of memory block

};

Block::Block() {
	// emptyBlock
};

inline void Block::InitializeBlock(const size_t& size)
{
	blockStart = std::malloc(size + sizeof(size_t));
	((size_t*)blockStart)[0] = size;
}

Block::Block(size_t size)  {
	InitializeBlock(size);
}

inline size_t Block::Size() const
{
	if (Initialized())
		return ((size_t*)blockStart)[0];
	return 0;
}

inline bool Block::Initialized() const
{
	return blockStart;
}

inline void Block::Grow()
{
	size_t s = Size();
	if (s)
		Grow((size_t)((double)s * 1.62));
}

inline void Block::Grow(size_t newSize)
{
	size_t oldSize = Size();
	if (newSize > oldSize)
	{
		void* oldBlock = blockStart;
		void* oldStart = memStart();
		InitializeBlock(newSize);
		memcpy(memStart(), oldStart, oldSize);
		free(oldBlock);
	}
}
inline void* Block::memStart()
{
	return (uint8_t*)blockStart + sizeof(size_t);
}
;

