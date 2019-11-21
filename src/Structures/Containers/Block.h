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


class Block : public iBlock {
public:
	Block();
	~Block();
	inline void InitializeBlock(const size_t& size);
	Block(const size_t& size);
	
	bool Initialized() const;
	size_t Size() const override;
	void Grow() override;
	void Grow(const size_t& newSize) override;
	void * memStart() override;
	const void* memStart() const override;


	void FastCopy(uint8_t* src, size_t StartIndex, size_t numBytes); //copy bytes into block
	void FastCopy(Block& src, size_t srcStartIndex, size_t StartIndex, size_t numBytes); //copy block into block
private:
	void* blockStart = 0;
	//dynamic size is stored inline at beggining of memory block

};

Block::Block() {
};
Block::~Block()
{
	if (blockStart)
		free(blockStart);
}

inline void Block::InitializeBlock(const size_t& size)
{
	blockStart = std::malloc(size + sizeof(size_t));
	((size_t*)blockStart)[0] = size;
	if (size > 0) {
		((baseTypes*)memStart())[0] = baseTypes::Void; //empty block
	}
}

Block::Block(const size_t& size)  {
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
		Grow((size_t)((double)s * 1.618));
	else
		Grow(9);
	

}

inline void Block::Grow(const size_t& newSize)
{ 
	size_t oldSize = Size();
	if (newSize > oldSize)
	{
		void* oldBlock = blockStart;
		void* oldStart = memStart();
		InitializeBlock(newSize);
		if (oldBlock) {// = 0 if was not initialized
			memcpy(memStart(), oldStart, oldSize);
			free(oldBlock);
		}
	}
}
inline void* Block::memStart()
{
	return (uint8_t*)blockStart + sizeof(size_t);
}
inline const void* Block::memStart() const
{
	return (uint8_t*)blockStart + sizeof(size_t);;
}
;

