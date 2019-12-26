#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Block
    @ingroup Block
    @brief inline Block of dynamic Memory, Self Allocating on heap
*/

#include "Structures/Types.h"
#include "Interfaces/iBlock.h"
#include <string.h>
#include <memory>



template <size_t SIZE>
class Block : public iBlock {
public:
	Block() {};
	Block(const iBlock& rhs) { Copy(rhs); };
	virtual size_t Capacity() const override { return SIZE; };
	size_t Size() const override { return SIZE; };
	virtual inline void* memStart() override { return &block; };
	virtual inline const void* memStart() const override { return &block; };
	void operator=(const Block& rhs) {Copy(rhs);};
	
private:
	uint8_t block[SIZE];
	
};

class DBlock : public iDBlock {
public:
	DBlock();
	DBlock(const size_t& size);
	DBlock(const DBlock& rhs);
	DBlock(DBlock&& rhs);
	~DBlock();
	
	inline void InitializeBlock(const size_t& size);
	inline bool Initialized() const { return blockStart; };	
	/// move-assignment operator (same capacity and size)
	void operator=(const DBlock& rhs) { copy(rhs); };
	/// move-assignment operator (same capacity and size)
	void operator=(DBlock&& rhs) noexcept { move(std::move(rhs)); };
	
	virtual void GrowMap(const CopyMap& map) override;
	virtual size_t Capacity() const override { return size; };
	virtual inline size_t Size() const override { return size; };
	virtual inline void* memStart() override { return blockStart; };
	virtual inline const void* memStart() const override { return blockStart; };
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
	size = 0;
}

inline void DBlock::InitializeBlock(const size_t& newSize)
{
	size = newSize;
	blockStart = newSize ? std::malloc(size) : 0;
}

DBlock::DBlock(const size_t& size)  {
	InitializeBlock(size);
}

inline void DBlock::GrowMap(const CopyMap& map)
{
	void* oldBlock = memStart();
	InitializeBlock(map.newSize);
	if (oldBlock) {
		map.CopyTo((uint8_t*)memStart());
		free(oldBlock);
	}
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
};


