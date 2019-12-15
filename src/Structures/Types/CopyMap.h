//------------------------------------------------------------------------------
//  CopyMap.h
//------------------------------------------------------------------------------

#include <memory>

struct CopyRange {
	size_t dstOffset;
	void* src;
	size_t size;
};

class CopyMap {
public:
	size_t newSize = 0;
	size_t RangeCount = 0;
	CopyRange* ranges;
	//add one more range to the array
	void PushRange(const CopyRange& range);
	void CopyTo(uint8_t* dst) const;
	void MoveTo(uint8_t* dst) const;
private:
	//calculate size of current memory
	size_t MemSize();
	//GrowEnough to add one more range if nessacary
	void Grow();
};

inline void CopyMap::PushRange(const CopyRange& range)
{
	Grow();
	ranges[RangeCount] = range;
	RangeCount++;
}

inline void CopyMap::CopyTo(uint8_t* dst) const
{
	const CopyRange* end = ranges + RangeCount;
	for (const CopyRange* r = ranges; r < end; r++)
		memcpy(dst + r->dstOffset, r->src, r->size);
}

inline void CopyMap::MoveTo(uint8_t* dst) const 
{
	const CopyRange* end = ranges + RangeCount;
	for (const CopyRange* r = ranges; r < end; r++)
		memmove(dst + r->dstOffset, r->src, r->size);
}

inline size_t CopyMap::MemSize()
{
	size_t memSize = 0;
	while (memSize < RangeCount * sizeof(CopyRange))
		memSize += 1024;

	return memSize;
}

void CopyMap::Grow()
{
	size_t memSize = MemSize();
	if ((RangeCount + 1) * sizeof(CopyRange) < memSize)
		return;//no need to grow

	//keep heap allocations always base 2 alligned, avoids memory fragmentation
	void* oldRanges = ranges;
	ranges = (CopyRange*)_aligned_malloc(memSize + 1024, 1024);
	if (oldRanges) {
		memcpy(ranges, oldRanges, RangeCount * sizeof(CopyRange));
		free(oldRanges);
	}
}