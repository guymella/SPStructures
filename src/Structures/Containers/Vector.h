//------------------------------------------------------------------------------
//  Vector.h
//------------------------------------------------------------------------------

#include "Structures/Interfaces/iArray.h"
#include <memory>

template<typename TYPE,size_t ALLIGNMENTLIMIT = 4096>
class Vector : public iDDArray<TYPE> {
public:
	size_t Size() const { return size; };	
	/// pop the last element
	TYPE PopBack() override = 0;
	TYPE PopBack(size_t numElements) override = 0;
	/// pop the first element
	TYPE PopFront() override = 0;
	TYPE PopFront(size_t numElements) override = 0;
private:
	size_t size=0;
	TYPE* data;
	//calculate size of current memory
	size_t MemSize();
	//GrowEnough to add one more range if nessacary
	void Grow();
};

