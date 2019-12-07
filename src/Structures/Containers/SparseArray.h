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
#include "Structures/Containers/SmartSlice.h"
#include "Structures/Interfaces/iArray.h"
#include "Structures/Containers/Block.h"

#include <initializer_list>

//namespace Structures {

template<class TYPE> class SparseArray : public iSparseArray<TYPE> 
{
public:
    /// default constructor
	SparseArray() { default = TYPE();  };
	SparseArray(TYPE defaultValue) { default = defaultValue; };
    /// copy constructor (truncates to actual size)
	SparseArray(const SparseArray& rhs);
    /// move constructor (same capacity and size)
	SparseArray(SparseArray&& rhs);
    /// initialize from initializer list
	SparseArray(std::initializer_list<TYPE> l);
    /// destructor
    //~SparseArray();

    /// copy-assignment operator (truncates to actual size)
    void operator=(const SparseArray& rhs);
    /// move-assignment operator (same capacity and size)
    void operator=(SparseArray&& rhs);
    
    /// get number of elements in array
	size_t Size() const override;
    /// read/write access an existing element
	TYPE& operator[](size_t index) override { return GetOrCreate(index); };
    /// read-only access to existing element
    const TYPE& operator[](size_t index) const override;
    /// read/write access to first element (must exists)
	TYPE& Front() override { return Values().Front(); };
    /// read-only access to first element (must exist)
    const TYPE& Front() const override { return Values().Front(); };
    /// read/write access to last element (must exist)
    TYPE& Back() override { return Values().Back(); };
    /// read-only access to last element (must exist)
    const TYPE& Back() const override { return Values().Back(); };
   
   	void ShiftRange(size_t StartIndex, size_t numElements, const int64_t& shiftAmmount ) override;

	/// clear the array (deletes elements, keeps capacity)
    void Clear(); //override TODO:: override after moving to iEditable
    
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
   
    /// erase a range of elements, keep element order
    void EraseRange(size_t index, size_t num) override;
    
    /// find element index with slow linear search, return InvalidIndex if not found
	//size_t FindIndexLinear(const TYPE& elm, size_t startIndex=0, size_t endIndex=std::numeric_limits<size_t>::max()) const;
    
	void insertBlank(const size_t& index, size_t count = 1) override;

    /// C++ conform begin
	TYPE* begin(const int64_t& offset = 0) override;
    /// C++ conform begin
	const TYPE* begin(const int64_t& offset = 0) const override;
    /// C++ conform end
	TYPE* end(const int64_t& offset = 0) override;
    /// C++ conform end
    const TYPE* end(const int64_t& offset = 0) const override;
    
	const TYPE* Exists(size_t index) const override;
	TYPE* Exists(size_t index) override;
private:
    /// destroy array resources
    //void destroy();
    /// copy from other array
    void copy(const SparseArray& rhs);
    /// move from other array
    void move(SparseArray&& rhs);
    /// reallocate with new capacity
    //void adjustCapacity(size_t newCapacity);
    
	TYPE& GetOrCreate(size_t index) override;
	iDArray<size_t>& Indexes() { return indexes; };
	const iDArray<size_t>& Indexes() const { return indexes; };
	size_t FindIndex(size_t index) const;	
	iDArray<TYPE>& Values() { return values; };
	const iDArray<TYPE>& Values() const { return values; };
	Array<TYPE> values;
	Array<size_t> indexes;
	TYPE default;
};

template<class TYPE>
inline SparseArray<TYPE>::SparseArray(const SparseArray& rhs)
{
	copy(rhs);
}

template<class TYPE>
inline SparseArray<TYPE>::SparseArray(SparseArray&& rhs)
{
	move(std::move(rhs));
}

template<class TYPE>
inline SparseArray<TYPE>::SparseArray(std::initializer_list<TYPE> l)
{
	Values().Reserve(int(l.size()));
	Indexes().Reserve(int(l.size()));
	for (const auto& elm : l) {
		PushBack(elm);
	}
}

template<class TYPE>
inline void SparseArray<TYPE>::operator=(const SparseArray& rhs)
{
	copy(rhs);
}

template<class TYPE>
inline void SparseArray<TYPE>::operator=(SparseArray&& rhs)
{
	move(std::move(rhs));
}

template<class TYPE>
inline size_t SparseArray<TYPE>::Size() const
{
	if (Indexes().Empty())
		return 0;
		
	return Indexes().Back() + 1;
}

template<class TYPE>
inline const TYPE& SparseArray<TYPE>::operator[](size_t index) const
{
	const TYPE* v = Exists(index);
	if (v)
		return *v;
	
	return default;
}

template<class TYPE>
inline void SparseArray<TYPE>::ShiftRange(size_t StartIndex, size_t numElements, const int64_t& shiftAmmount)
{
	size_t endIndex = StartIndex + numElements;
	size_t DeleteStart = StartIndex + shiftAmmount;
	size_t DeleteEnd = endIndex + shiftAmmount;

	if (shiftAmmount > 0 && shiftAmmount < (int64_t)numElements)
		DeleteStart = endIndex;
	else if (shiftAmmount < 0 && -shiftAmmount < (int64_t)numElements)
		DeleteEnd = StartIndex;



	for (size_t i = 0; i < Indexes().Size(); i++) {
		size_t& ii = Indexes()[i];
		if (ii < StartIndex && ii < DeleteStart) //skip
			continue;
		if (ii >= DeleteStart && ii < DeleteEnd) {//delete; should be overidden by shift
			Indexes().Erase(i);
			Values().Erase(i);
			i--;
			continue;
		}
		if (ii >= StartIndex && ii < endIndex)//do the shift
			ii += shiftAmmount;

		if (ii > endIndex&& ii > DeleteEnd)
			break;
	}
}

template<class TYPE>
inline void SparseArray<TYPE>::Clear()
{
	Values().Clear();
	Indexes().Clear();
}

template<class TYPE>
inline TYPE& SparseArray<TYPE>::PushBack(const TYPE& elm)
{
	Indexes().PushBack(Size());
	return Values().PushBack(elm);
}

template<class TYPE>
inline TYPE& SparseArray<TYPE>::PushBack(TYPE&& elm)
{
	Indexes().PushBack(Size());
	return Values().PushBack(std::move(elm));
}

template<class TYPE>
inline TYPE& SparseArray<TYPE>::PushFront(const TYPE& elm)
{
	for (size_t i = 0; i < Indexes().Size(); i++) {
		Indexes()[i]++;
	}
	Indexes().PushFront(0);
	return Values().PushFront(elm);
}

template<class TYPE>
inline TYPE& SparseArray<TYPE>::PushFront(TYPE&& elm)
{
	for (size_t i = 0; i < Indexes().Size(); i++) {
		Indexes()[i]++;
	}
	Indexes().PushFront(0);
	return Values().PushFront(std::move(elm));	
}

template<class TYPE>
inline void SparseArray<TYPE>::Insert(size_t index, const TYPE& elm)
{
    size_t i = FindIndex(index);

	Values().Insert(i, elm);
	Indexes().Insert(i++, index);

	for (i; i < Indexes().Size(); i++) {
		Indexes()[i]++;
	}
}

template<class TYPE>
inline void SparseArray<TYPE>::Insert(size_t index, TYPE&& elm)
{
	size_t i = FindIndex(index);

	Values().Insert(i, std::move(elm));
	Indexes().Insert(i++, index);

	for (i; i < Indexes().Size(); i++) {
		Indexes()[i]++;
	}
}

template<class TYPE>
inline TYPE SparseArray<TYPE>::PopBack()
{
	Indexes().PopBack();
	return Values().PopBack();
}

template<class TYPE>
inline TYPE SparseArray<TYPE>::PopBack(size_t numElements)
{
	Indexes().PopBack();
	return Values().PopBack();
}

template<class TYPE>
inline TYPE SparseArray<TYPE>::PopFront()
{
	Indexes().PopFront();
	for (size_t i = 0; i < Indexes().Size(); i++)
		Indexes()[i]--;

	return Values().PopFront();
}

template<class TYPE>
inline TYPE SparseArray<TYPE>::PopFront(size_t numElements)
{
	Indexes().PopFront();
	for (size_t i = 0; i < Indexes().Size(); i++)
		Indexes()[i]--;

	return Values().PopFront();
}

template<class TYPE>
inline void SparseArray<TYPE>::EraseRange(size_t index, size_t num)
{
	size_t b = FindIndex(index);
	size_t n = FindIndex(index + num)-b;
	
	Indexes().EraseRange(b, n);
	Values().EraseRange(b, n);

	for (size_t i = b; i < Indexes().Size(); i++) {
		Indexes()[i] -= num;
	}
}

template<class TYPE>
inline void SparseArray<TYPE>::insertBlank(const size_t& index, size_t count)
{
	for (size_t i = FindIndex(index); i < Indexes().Size(); i++)
		Indexes()[i] += count;
}
template<class TYPE>
inline TYPE* SparseArray<TYPE>::begin(const int64_t& offset)
{
	return (Values().begin(FindIndex(offset)));
}

template<class TYPE>
inline const TYPE* SparseArray<TYPE>::begin(const int64_t& offset) const
{
	return (Values().begin(FindIndex(offset)));
}

template<class TYPE>
inline TYPE* SparseArray<TYPE>::end(const int64_t& offset)
{
	return (Values().end(FindIndex(offset)));
}

template<class TYPE>
inline const TYPE* SparseArray<TYPE>::end(const int64_t& offset) const
{
	return (Values().end(FindIndex(offset)));
}


template<class TYPE>
inline void SparseArray<TYPE>::copy(const SparseArray& rhs)
{
	values = rhs.values;
	indexes = rhs.indexes;
}

template<class TYPE>
inline void SparseArray<TYPE>::move(SparseArray&& rhs)
{
	values = std::move(rhs.values);
	indexes = std::move(rhs.indexes);
}

template<class TYPE>
inline TYPE& SparseArray<TYPE>::GetOrCreate(size_t index)
{
	if (!Size()) {
		Indexes().PushBack(index);
		return Values().PushBack(default);
	}
	
	size_t i = FindIndex(index);

	if (Indexes()[i] == index)
		return Values()[i];

	Indexes().Insert(i, index);
	Values().Insert(i, default);
	return Values()[i];
}

template<class TYPE>
inline size_t SparseArray<TYPE>::FindIndex(size_t index) const
{
	for (size_t i = 0; i < Indexes().Size(); i++)
		if (Indexes()[i] >= index)
			return i;

	return Indexes().Size();
}

template<class TYPE>
inline const TYPE* SparseArray<TYPE>::Exists(size_t index) const
{
	if (!Size()) 
		return NULL;

	size_t i = FindIndex(index);

	if (Indexes()[i] == index)
		return &Values()[i];

	return NULL;
}

template<class TYPE>
inline TYPE* SparseArray<TYPE>::Exists(size_t index)
{
	if (!Size()) 
		return NULL;

	size_t i = FindIndex(index);

	if (Indexes()[i] == index)
		return &Values()[i];

	return NULL;
}
