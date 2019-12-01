#pragma once
//------------------------------------------------------------------------------
/**
    @class Oryol::Array
    @ingroup Core
    @brief dynamic array class similar to std::vector
    
    Dynamic array class (similar to std::vector) with room for growth at
    the end. Internally this is a double-ended array, e.g. erasing items
    at the front does not involve a move-operation, and erasing in the
    middle will move either from the front or the back, depending on what
    would involve less move ops, or use EraseSwap() if the element
    ordering is not important. If there is no spare room at the front
    (this is the is always the case, unless an element has been
    removed from the front), insertion at the front will involve
    size() move operations.
 
    When working with pointers or references into the array keep in mind
    that operations which add new elements may reallocate the whole array
    at a different memory location!
    
    NOTE: An array *copy* operation will trim the capacity to the
    number of elements, so that no spare room is in the array. A *move*
    will not truncate but keep the previous size and capacity.
    
    NOTE: An array growth operation will truncate any spare room
    at the front.
    
    For sorting, iterating and sorted insertion, use the standard 
    algorithm stuff!
    
    @see ArrayMap, Map, Set, HashSet
*/
//#include "Core/Config.h"
//#include "Core/Containers/elementBuffer.h"
#include "Structures/Containers/Slice.h"
#include "Structures/Interfaces/iArray.h"
#include "Structures/Containers/Block.h"

#include <initializer_list>

//namespace Structures {

template<class TYPE> class Array : public iDArray<TYPE>, public iSliceable<TYPE>{
public:
    /// default constructor
    Array();
    /// copy constructor (truncates to actual size)
    Array(const Array& rhs);
    /// move constructor (same capacity and size)
    Array(Array&& rhs);
    /// initialize from initializer list
    Array(std::initializer_list<TYPE> l);
    /// destructor
    ~Array();

    /// copy-assignment operator (truncates to actual size)
    void operator=(const Array& rhs);
    /// move-assignment operator (same capacity and size)
    void operator=(Array&& rhs);
    
    /// set allocation strategy
    //void SetAllocStrategy(int minGrow_, int maxGrow_=ORYOL_CONTAINER_DEFAULT_MAX_GROW);
    /// initialize the array to a fixed capacity (guarantees that no re-allocs happen)
    //void SetFixedCapacity(int fixedCapacity);
    /// get min grow value
    //int GetMinGrow() const;
    /// get max grow value
    //int GetMaxGrow() const;
    /// get number of elements in array
    size_t Size() const override;
    /// return true if empty
    //bool Empty() const override;
	using iDArray<TYPE>::Empty;
    /// get capacity of array
    size_t Capacity() const override;
    /// get number of free slots at back of array
    size_t Spare() const override;
	size_t SpareFront() const override;
	
	size_t SpareBack() const override;
    
    /// read/write access an existing element
    TYPE& operator[](size_t index) override;
    /// read-only access to existing element
    const TYPE& operator[](size_t index) const override;
    /// read/write access to first element (must exists)
    TYPE& Front() override;
    /// read-only access to first element (must exist)
    const TYPE& Front() const override;
    /// read/write access to last element (must exist)
    TYPE& Back() override;
    /// read-only access to last element (must exist)
    const TYPE& Back() const override;
    /// get a slice into the array (beware of iterator-invalidation!)
    Slice<TYPE> MakeSlice(size_t offset=0, size_t numItems= std::numeric_limits<size_t>::max()) override;

    /// increase capacity to hold at least numElements more elements
	void ReserveFront(const size_t& numElements) override;
	void ReserveBack(const size_t& numElements) override;
    void Reserve(const size_t& numElements) override;
	void Reserve(const size_t& numElementsFront, const size_t& numElementsBack) override;
	void ShiftBack(const size_t& numShift) override; //free up spare without realloc
	void ShiftFront(const size_t& numShift) override;
	void ShiftRange(size_t StartIndex, size_t numElements, const int64_t& shiftAmmount ) override;

	void Grow() override; // just grow (very dumb grow = golden ratio)
	void Grow(const size_t& newSize) override; //grow to specific size, copy all to front (dumb grow)
	void Grow(const size_t& newSize, const size_t& frontPorch) override;
    /// trim capacity to size (this involves a re-alloc)
    void Trim() override;
    /// clear the array (deletes elements, keeps capacity)
    void Clear() override;
    
    /// copy-add element to back of array
    TYPE& PushBack(const TYPE& elm);
    /// move-add element to back of array
    TYPE& PushBack(TYPE&& elm);
	/// copy-add element to back of array
	TYPE& PushFront(const TYPE& elm);
	/// move-add element to back of array
	TYPE& PushFront(TYPE&& elm);
    /// construct-add new element at back of array
 //   template<class... ARGS> TYPE& PushBack(ARGS&&... args);
	//template<class... ARGS> TYPE& PushFront(ARGS&&... args);
    /// copy-insert element at index, keep array order
    void Insert(size_t index, const TYPE& elm) override;
    /// move-insert element at index, keep array order
    void Insert(size_t index, TYPE&& elm) override;

    /// pop the last element
    TYPE PopBack() override;
	TYPE PopBack(size_t numElements) override;
    /// pop the first element
    TYPE PopFront() override;
	TYPE PopFront(size_t numElements) override;
    /// erase element at index, keep element order
    void Erase(size_t index) override;
    /// erase element at index, swap-in front or back element (destroys element ordering)
    void EraseSwap(size_t index) override;
    /// erase element at index, always swap-in from back (destroys element ordering)
    void EraseSwapBack(size_t index) override;
    /// erase element at index, always swap-in from front (destroys element ordering)
    void EraseSwapFront(size_t index) override;
    /// erase a range of elements, keep element order
    void EraseRange(size_t index, size_t num) override;
    
    /// find element index with slow linear search, return InvalidIndex if not found
	//size_t FindIndexLinear(const TYPE& elm, size_t startIndex=0, size_t endIndex=std::numeric_limits<size_t>::max()) const;
    
	void insertBlank(const size_t& index, size_t count = 1);

    /// C++ conform begin
    TYPE* begin(const int64_t& offset = 0) override;
    /// C++ conform begin
    const TYPE* begin(const int64_t& offset = 0) const override;
    /// C++ conform end
    TYPE* end(const int64_t& offset = 0) override;
    /// C++ conform end
    const TYPE* end(const int64_t& offset = 0) const override;
    
private:
    /// destroy array resources
    void destroy();
    /// copy from other array
    void copy(const Array& rhs);
    /// move from other array
    void move(Array&& rhs);
    /// reallocate with new capacity
    void adjustCapacity(size_t newCapacity);
    

	iDBlock* Block();
	const iDBlock* Block() const;

	size_t maxGrow();
    
	size_t startElem = 0;
	size_t size = 0;
    size_t minGrow = 128;
    //size_t maxGrow;

	DBlock memoryBlock;
	
};

//------------------------------------------------------------------------------
template<class TYPE>
Array<TYPE>::Array()  : minGrow(128)
{
    // empty
}

//------------------------------------------------------------------------------
template<class TYPE>
Array<TYPE>::Array(const Array& rhs) {
    this->copy(rhs);
}
//
////------------------------------------------------------------------------------
template<class TYPE>
Array<TYPE>::Array(Array&& rhs) {
    this->move(std::move(rhs));
}
//
////------------------------------------------------------------------------------
template<class TYPE>
Array<TYPE>::Array(std::initializer_list<TYPE> l) :minGrow(128) {
    this->Reserve(int(l.size()));
    for (const auto& elm : l) {
        this->PushBack(elm);
    }
}

//------------------------------------------------------------------------------
template<class TYPE>
Array<TYPE>::~Array() {
    this->destroy();
};

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::operator=(const Array<TYPE>& rhs) {
    /// @todo: this should be optimized when rhs.size() < this->capacity()!
    if (&rhs != this) {
        this->destroy();
        this->copy(rhs);
    }
}
//
////------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::operator=(Array<TYPE>&& rhs) {
    /// @todo: this should be optimized when rhs.size() < this->capacity()!
    if (&rhs != this) {
        this->destroy();
        this->move(std::move(rhs));
    }
}

////------------------------------------------------------------------------------
//template<class TYPE> void
//Array<TYPE>::SetAllocStrategy(int minGrow_, int maxGrow_) {
//    this->minGrow = minGrow_;
//    this->maxGrow = maxGrow_;
//}
//
////------------------------------------------------------------------------------
//template<class TYPE> void
//Array<TYPE>::SetFixedCapacity(int fixedCapacity) {
//    this->minGrow = 0;
//    this->maxGrow = 0;
//    if (fixedCapacity > this->buffer.capacity()) {
//        this->adjustCapacity(fixedCapacity);
//    }
//}
//
////------------------------------------------------------------------------------
//template<class TYPE> int
//Array<TYPE>::GetMinGrow() const {
//        return this->minGrow;
//    }
//    
////------------------------------------------------------------------------------
//template<class TYPE> int
//Array<TYPE>::GetMaxGrow() const {
//    return this->maxGrow;
//}

//------------------------------------------------------------------------------
template<class TYPE> size_t
Array<TYPE>::Size() const {
	return size;
}

//------------------------------------------------------------------------------
template<class TYPE> size_t
Array<TYPE>::Capacity() const {
    return Block()->Size() / sizeof(TYPE);
}

//------------------------------------------------------------------------------
template<class TYPE> size_t
Array<TYPE>::Spare() const {
	return SpareBack();
}

template<class TYPE>
inline size_t Array<TYPE>::SpareFront() const
{
	return startElem;
}

template<class TYPE>
inline size_t Array<TYPE>::SpareBack() const
{
	return Capacity()-Size()-startElem;
}

//------------------------------------------------------------------------------
template<class TYPE> TYPE&
Array<TYPE>::operator[](size_t index) {
	//TODO:: handle out of range
	return begin()[index];
}

//------------------------------------------------------------------------------
template<class TYPE> const TYPE&
Array<TYPE>::operator[](size_t index) const {
	//TODO:: handle out of range
    return begin()[index];
}

//------------------------------------------------------------------------------
template<class TYPE> TYPE&
Array<TYPE>::Front() {
	//TODO:: handle empty
    return *begin();
}

//------------------------------------------------------------------------------
template<class TYPE> const TYPE&
Array<TYPE>::Front() const {
	//TODO:: handle empty
    return *begin();
}

//------------------------------------------------------------------------------
template<class TYPE> TYPE&
Array<TYPE>::Back() {
	//TODO:: handle empty
	TYPE* e = end();
	return *(--e);
}

//------------------------------------------------------------------------------
template<class TYPE> const TYPE&
Array<TYPE>::Back() const {
	//TODO:: handle empty
	const TYPE* e = end();
    return *(--e);
}

//------------------------------------------------------------------------------
template<class TYPE> Slice<TYPE>
Array<TYPE>::MakeSlice(size_t offset, size_t numItems) {
    if (offset+numItems > Size()) {
        numItems = Size() - offset;
    }
    return Slice<TYPE>(begin(), Size(), offset, numItems);
}

template<class TYPE>
inline void Array<TYPE>::ReserveFront(const size_t& numElements)
{
	//TODO::
}

template<class TYPE>
inline void Array<TYPE>::ReserveBack(const size_t& numElements)
{
	//TODO::
}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::Reserve(const size_t& numElements) {
    size_t newCapacity = Size() + numElements;
    if (newCapacity > Capacity()) {
        this->adjustCapacity(newCapacity);
    }
}

template<class TYPE>
inline void Array<TYPE>::Reserve(const size_t& numElementsFront, const size_t& numElementsBack)
{
	//TODO::
}

template<class TYPE>
inline void Array<TYPE>::ShiftBack(const size_t& numShift)
{
	//TODO::
}

template<class TYPE>
inline void Array<TYPE>::ShiftFront(const size_t& numShift)
{
	//TODO::
}

template<class TYPE>
inline void Array<TYPE>::ShiftRange(size_t StartIndex, size_t numElements, const int64_t& shiftAmmount)
{
	if (StartIndex >= Size())
		return;

	if (StartIndex + numElements > Size())
		numElements = Size() - StartIndex;

	int64_t ff = StartIndex + numElements + shiftAmmount;
	if (ff > (int64_t)Size())
		numElements -= ff - Size();

	ff = StartIndex + shiftAmmount;
	if (ff < 0)
		StartIndex += abs(ff);

	TYPE* src = begin() + StartIndex;
	void* dst = src + shiftAmmount;
	std::memmove(dst, src, numElements * sizeof(TYPE));

	if (shiftAmmount < 0) {
		src += numElements + shiftAmmount;
	}
	
	//clear gap
	std::memset(src, 0, abs(shiftAmmount) * sizeof(TYPE));
}

template<class TYPE>
inline void Array<TYPE>::Grow()
{
	if (!Block()->Size()) {
		Grow(minGrow);
	} else {
		size_t newCap = (size_t)((double)Capacity() * 1.618);
		Grow(newCap);
	}
}

template<class TYPE>
inline void Array<TYPE>::Grow(const size_t& newSize)
{
	Grow(newSize, 0);
}

template<class TYPE>
inline void Array<TYPE>::Grow(const size_t& newCap, const size_t& frontPorch)
{
	size_t newFP = newCap / 4;
	newFP -= SpareFront();
	newFP += frontPorch;
	Block()->Grow(newCap * sizeof(TYPE), newFP * sizeof(TYPE));
	startElem += newFP;
}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::Trim() {
	CopyRange cpy;
	cpy.dstOffset = 0;
	cpy.size = Size() * sizeof(TYPE);
	cpy.src = begin();
	Block()->GrowCopyMap(cpy.size, &cpy, 1);
	startElem = 0;

}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::Clear() {
	size = 0;
}

//------------------------------------------------------------------------------
template<class TYPE> TYPE&
Array<TYPE>::PushBack(const TYPE& elm) {
    if (!SpareBack()) {
		Grow();
    }
	size++;
	Back() = elm;
    return Back();
}

//------------------------------------------------------------------------------
template<class TYPE> TYPE&
Array<TYPE>::PushBack(TYPE&& elm) {
	if (!SpareBack()) {
		Grow();
	}
	size++;
	Back() = std::move(elm);
	TYPE db = Back();
	return Back();
}

template<class TYPE>
inline TYPE& Array<TYPE>::PushFront(const TYPE& elm)
{
	if (!SpareFront()) {
		Grow();
	}
	startElem--;
	size++;
	Front() = elm;
	return Front();
}

template<class TYPE>
inline TYPE& Array<TYPE>::PushFront(TYPE&& elm)
{
	if (!SpareFront()) {
		Grow();
	}
	startElem--;
	size++;
	Front() = std::move(elm);
	return Front();
}
    
//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::Insert(size_t index, const TYPE& elm) {
	insertBlank(index);
	begin()[index] = elm;
}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::Insert(size_t index, TYPE&& elm) {
	insertBlank(index);
	begin()[index] = std::move(elm);
}

//------------------------------------------------------------------------------
//template<class TYPE> template<class... ARGS> TYPE&
//Array<TYPE>::PushBack(ARGS&&... args) {
//	if (!SpareBack())
//		Grow();
//	size++;
//	Back() = std::forward<ARGS>(args)...;
//    return Back();
//}
//
//template<class TYPE>
//template<class ...ARGS>
//inline TYPE& Array<TYPE>::PushFront(ARGS&& ...args)
//{
//	if (!SpareFront())
//		Grow();
//
//	startElem--;
//	size++;
//	Front() = std::forward<ARGS>(args)...;
//	return Front();
//}

//------------------------------------------------------------------------------
template<class TYPE> TYPE
Array<TYPE>::PopBack() {
	TYPE * e = end();
	e--;
	size--;
    return *e;
}

template<class TYPE>
inline TYPE Array<TYPE>::PopBack(size_t numElements)
{
	//TODO::return array with elements popped off
	return TYPE();
}

//------------------------------------------------------------------------------
template<class TYPE> TYPE
Array<TYPE>::PopFront() {
	TYPE * b = begin();
	startElem++;	
	size--;	
    return *b;
}

template<class TYPE>
inline TYPE Array<TYPE>::PopFront(size_t numElements)
{
	//TODO::
	return TYPE();
}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::Erase(size_t index) {
	EraseRange(index, 1);
}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::EraseSwap(size_t index) {
	EraseSwapBack(index);
}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::EraseSwapBack(size_t index) {
	begin()[index] = PopBack();
}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::EraseSwapFront(size_t index) {
	begin()[index-1] = PopFront();
}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::EraseRange(size_t index, size_t num) {
	if (index + num < Size())
		ShiftRange(index + num, Size(), -(int64_t)num);
	size-=num;
}

//------------------------------------------------------------------------------
//template<class TYPE> size_t
//Array<TYPE>::FindIndexLinear(const TYPE& elm, size_t startIndex, size_t endIndex) const {
//	if (!Empty()) {
//        if (endIndex > Size()) 
//            endIndex = Size();
//        
//		const TYPE* bgn = begin();
//        for (size_t i = startIndex; i < endIndex; i++) {
//            if (elm == bgn[i]) {
//                return i;
//            }
//        }
//    }
//    // fallthrough: not found
//    return 0;
//}

template<class TYPE>
inline void Array<TYPE>::insertBlank(const size_t& index, size_t count)
{
	bool canShiftFwd = SpareBack() > count;
	bool canShiftBck = SpareFront() > count;

	if (canShiftFwd || canShiftBck) {
		//do the shift
		bool shiftfwd = (index >= Size() / 2);
		if (shiftfwd && canShiftFwd || !shiftfwd && !canShiftBck) {
			//shift forward
			size_t ss = Size() - index;
			TYPE* src = begin() + index;
			void* dst = src + count;
			std::memmove(dst, src, ss * sizeof(TYPE));
		}
		else
		{
			//shiftback
			size_t ss = index;
			TYPE* src = begin();
			void* dst = src - count;
			std::memmove(dst, src, index*sizeof(TYPE));
			startElem -= count;
		}		
	}
	else {
		CopyRange copyMap[2];
		size_t newSize = Size() + count;
		size_t newFP = newSize / 4;
		newSize += newFP * 3;

		copyMap[0].dstOffset = sizeof(TYPE) * newFP;
		copyMap[0].size = index * sizeof(TYPE);
		copyMap[0].src = begin();
		copyMap[1].dstOffset = (newFP + index + count) *sizeof(TYPE);
		copyMap[1].size = (Size() - index) * sizeof(TYPE);
		copyMap[1].src = begin()+index;

		Block()->GrowCopyMap(newSize, copyMap, 2);
		startElem = newFP;
	}

	size += count;
	
}
    
//------------------------------------------------------------------------------
template<class TYPE> TYPE*
Array<TYPE>::begin(const int64_t& offset) {
	return ((TYPE*)(Block()->memStart())) + startElem+offset;
}

//------------------------------------------------------------------------------
template<class TYPE> const TYPE*
Array<TYPE>::begin(const int64_t& offset) const {
    return ((TYPE*)(Block()->memStart())) + startElem + offset;
}

//------------------------------------------------------------------------------
template<class TYPE> TYPE*
Array<TYPE>::end(const int64_t& offset) {
    return begin(offset) +size;
}

//------------------------------------------------------------------------------
template<class TYPE> const TYPE*
Array<TYPE>::end(const int64_t& offset) const {
    return begin(offset) + size;
}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::destroy() {
    minGrow = 0;
    //maxGrow = 0;
	startElem = 0;
	size = 0;

	//TODO:: dealocate Block
}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::copy(const Array& rhs) {
    minGrow = rhs.minGrow;
    //maxGrow = rhs.maxGrow;
	startElem = rhs.startElem;
	size = rhs.size;

	memoryBlock = rhs.memoryBlock;

}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::move(Array&& rhs) {
    minGrow = rhs.minGrow;
    //maxGrow = rhs.maxGrow;
	startElem = rhs.startElem;
	size = rhs.size;
	memoryBlock = std::move(rhs.memoryBlock);
    // NOTE: don't reset minGrow/maxGrow, rhs is empty, but still a valid object!
	rhs.size = 0;
	rhs.startElem = 0;
}

//------------------------------------------------------------------------------
template<class TYPE> void
Array<TYPE>::adjustCapacity(size_t newCapacity) {
	//TODO::
	Grow(newCapacity);
}

template<class TYPE>
inline iDBlock* Array<TYPE>::Block()
{
	return &memoryBlock;
}

template<class TYPE>
inline const iDBlock* Array<TYPE>::Block() const
{
	return &memoryBlock;
}

template<class TYPE>
inline size_t Array<TYPE>::maxGrow()
{
	return (size_t)(((double)Size())*1.62);//golden ratio
}

//------------------------------------------------------------------------------
//template<class TYPE> void
//Array<TYPE>::grow() {
//    const int curCapacity = this->buffer.capacity();
//    int growBy = curCapacity >> 1;
//    if (growBy < minGrow) {
//        growBy = minGrow;
//    }
//    else if (growBy > maxGrow) {
//        growBy = maxGrow;
//    }
//    o_assert_dbg(growBy > 0);
//    int newCapacity = curCapacity + growBy;
//    this->adjustCapacity(newCapacity);
//}

//}
