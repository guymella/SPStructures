#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iIndexable
    @ingroup Interfaces
    @brief interface for structures that can be accessed by index
*/

template <typename TYPE>
class iIndexable {
public:
	virtual TYPE& operator[](size_t index) = 0;
	virtual const TYPE& operator[](size_t index) const = 0;
};

template <typename TYPE>
class iEditable {
public:
	/// copy-insert element at index, keep array order
	virtual void Insert(size_t index, const TYPE& elm) = 0;
	/// move-insert element at index, keep array order
	virtual void Insert(size_t index, TYPE&& elm) = 0;
	virtual void insertBlank(const size_t& index, size_t count = 1) = 0;

	/// pop the last element
	virtual TYPE PopBack() = 0;
	/// pop the first element
	virtual TYPE PopFront() = 0;
	/// erase element at index, keep element order
	virtual void Erase(size_t index) = 0;
	/// erase element at index, swap-in front or back element (destroys element ordering)
	virtual void EraseSwap(size_t index) = 0;
	/// erase element at index, always swap-in from back (destroys element ordering)
	virtual void EraseSwapBack(size_t index) = 0;
	/// erase element at index, always swap-in from front (destroys element ordering)
	virtual void EraseSwapFront(size_t index) = 0;
	/// erase a range of elements, keep element order
	virtual void EraseRange(size_t index, size_t num) = 0;

	virtual void ShiftRange(size_t StartIndex, size_t numElements, const int64_t& shiftAmmount) = 0;

	virtual void Clear() = 0;
};

template <typename TYPE>
class iSearchable {
public:
	/// find element index with slow linear search, return InvalidIndex if not found
	virtual size_t FindIndexLinear(const TYPE& elm, size_t startIndex, size_t endIndex) const =0;
};
