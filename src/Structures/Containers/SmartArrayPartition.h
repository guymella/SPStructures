#pragma once
//------------------------------------------------------------------------------
/**
	@class Oryol::Slice
	@ingroup Core
	@brief a reference to an array-like range of non-owned items

	An Slice<TYPE> is a generic reference to a number of array items
	which are owned by someone else. They are especially useful as function
	parameters or return values (just keep in mind that the referenced data is
	not owed, and the Slice may be affected by iterator-invalidation!)
*/
//#include "Core/Config.h"
//#include "Core/Assertion.h"
#include "Structures/Interfaces/iArray.h"
#include "Slice.h"

//namespace Oryol {
template<typename TYPE> class SmartSlice;

template<typename TYPE>
class iSmartSliceable {
public:
	virtual SmartSlice<TYPE> MakeSmartSlice(size_t sliceOffset = 0, size_t numSliceItems = std::numeric_limits<size_t>::max()) = 0;
};

template<typename TYPE>
class iSmartArrayPartition : public iDArray<TYPE>, public iSmartSliceable<TYPE>, public iSliceable<TYPE> {
public:
	/*/// read/write access to indexed item
	TYPE& operator[](size_t index) override;
	/// read-only access to indexed item
	const TYPE& operator[](size_t index) const override;*/
	/// create a new slice from this slice
	SmartSlice<TYPE> MakeSmartSlice(size_t sliceOffset = 0, size_t numSliceItems = std::numeric_limits<size_t>::max()) override;
	Slice<TYPE> MakeSlice(size_t sliceOffset = 0, size_t numSliceItems = std::numeric_limits<size_t>::max()) override;

	/// get the start index
	virtual size_t Offset() const = 0;
	//Get the base pointer
	virtual TYPE* BasePointer();
	virtual const TYPE* BasePointer() const;
	virtual iDArray<TYPE>* Base() = 0;
	virtual const iDArray<TYPE>* Base() const =0;
	virtual void ChangeSize(int64_t delta) = 0;
	/// copy-add element to back of array
	virtual TYPE& PushBack(const TYPE& elm) override;
	/// move-add element to back of array
	virtual TYPE& PushBack(TYPE&& elm) override;
	/// copy-add element to back of array
	virtual TYPE& PushFront(const TYPE& elm) override;
	/// move-add element to back of array
	virtual TYPE& PushFront(TYPE&& elm) override;
	/// construct-add new element at back of array
	//template<class... ARGS> TYPE& PushBack(ARGS&&... args);
	//template<class... ARGS> TYPE& PushFront(ARGS&&... args);
	/// copy-insert element at index, keep array order
	virtual void Insert(size_t index, const TYPE& elm) override;
	/// move-insert element at index, keep array order
	virtual void Insert(size_t index, TYPE&& elm) override;

	/// pop the last element
	virtual TYPE PopBack() override;
	virtual TYPE PopBack(size_t numElements) override;
	/// pop the first element
	virtual TYPE PopFront() override;
	virtual TYPE PopFront(size_t numElements) override;
	/*/// erase element at index, keep element order
	virtual void Erase(size_t index) override;
	/// erase element at index, swap-in front or back element (destroys element ordering)
	virtual void EraseSwap(size_t index) override;
	/// erase element at index, always swap-in from back (destroys element ordering)
	virtual void EraseSwapBack(size_t index) override;
	/// erase element at index, always swap-in from front (destroys element ordering)
	virtual void EraseSwapFront(size_t index) override;*/
	/// erase a range of elements, keep element order
	virtual void EraseRange(size_t index, size_t num) override;
	virtual void ShiftRange(size_t StartIndex, size_t numElements, const int64_t& shiftAmmount) override;

	virtual void insertBlank(const size_t& index, size_t count = 1) override;

	/// C++ begin
	TYPE* begin(const int64_t& offset = 0) override;
	/// C++ begin
	const TYPE* begin(const int64_t& offset = 0) const override;
	/// C++ end
	TYPE* end(const int64_t& offset = 0) override;
	/// C++ end
	const TYPE* end(const int64_t& offset = 0) const override;
};

//template<typename TYPE>
//class SmartArrayPartition : public iArrayPartition<TYPE> {
//public:
//	/// default constructor
//	ArrayPartition();
//	/// init from base pointer, start index and number of items
//	ArrayPartition(TYPE* base, size_t numBaseItems, size_t sliceOffset = 0, size_t numSliceItems = std::numeric_limits<size_t>::max());
//	/// copy constructor
//	ArrayPartition(const ArrayPartition<TYPE>& rhs);
//	/// Equality
//	bool operator==(const ArrayPartition<TYPE>& rhs);
//	/// copy-assignment
//	void operator=(const ArrayPartition<TYPE>& rhs);
//	/// get number of items
//	size_t Size() const override;
//	/// get the start index
//	virtual size_t Offset() const override;
//	//Get the base pointer
//	virtual TYPE* BasePointer() override;
//	virtual const TYPE* BasePointer() const override;
//
//protected:
//	iDArray* basePtr = nullptr;
//	size_t offset = 0;
//	size_t num = 0;
//};




//template<typename TYPE>
//inline ArrayPartition<TYPE>::ArrayPartition()
//{
//}
//
////------------------------------------------------------------------------------
//template<typename TYPE>
//ArrayPartition<TYPE>::ArrayPartition(TYPE* base, size_t numBaseItems, size_t sliceOffset, size_t sliceNumItems) :
//	basePtr(base),
//	offset(sliceOffset),
//	num((sliceOffset + sliceNumItems > numBaseItems) ? numBaseItems : sliceNumItems)
//{
//	// o_assert_dbg(basePtr && (offset>=0) && (num>=0) && ((offset+num)<=baseSize));
//}
//
//template<typename TYPE>
//inline ArrayPartition<TYPE>::ArrayPartition(const ArrayPartition<TYPE>& rhs)
//	: basePtr(rhs.basePtr), offset(rhs.offset), num(rhs.num)
//{
//}
//
////------------------------------------------------------------------------------
//template<typename TYPE> void
//ArrayPartition<TYPE>::operator=(const ArrayPartition<TYPE>& rhs) {
//	this->basePtr = rhs.basePtr;
//	this->baseSize = rhs.baseSize;
//	this->offset = rhs.offset;
//	this->num = rhs.num;
//}
//
//template<typename TYPE>
//inline bool ArrayPartition<TYPE>::operator==(const ArrayPartition<TYPE>& rhs)
//{
//	return (basePtr == rhs.basePtr && baseSize == rhs.baseSize && num == rhs.num && offset == rhs.offset);
//}

////------------------------------------------------------------------------------
//template<typename TYPE> TYPE&
//iSmartArrayPartition<TYPE>::operator[](size_t index) {
//	//o_assert_dbg(this->basePtr && (index >= 0) && (index < this->num));
//	return begin()[index];
//}
//
////------------------------------------------------------------------------------
//template<typename TYPE> const TYPE&
//iSmartArrayPartition<TYPE>::operator[](size_t index) const {
//	//o_assert_dbg(this->basePtr && (index >= 0) && (index < this->num));
//	return begin()[index];
//}

template<typename TYPE>
inline TYPE* iSmartArrayPartition<TYPE>::BasePointer()
{
	return Base()->begin();
}

template<typename TYPE>
inline const TYPE* iSmartArrayPartition<TYPE>::BasePointer() const
{
	return  Base()->begin();
}



//------------------------------------------------------------------------------
template<typename TYPE> SmartSlice<TYPE>
	iSmartArrayPartition<TYPE>::MakeSmartSlice(size_t sliceOffset, size_t numSliceItems) {
		if (sliceOffset + numSliceItems > Size()) {
			numSliceItems = Size() - sliceOffset;
		}
		return SmartSlice<TYPE>(Base(), Offset() + sliceOffset, numSliceItems);
	}

	template<typename TYPE>
	inline Slice<TYPE> iSmartArrayPartition<TYPE>::MakeSlice(size_t sliceOffset, size_t numSliceItems)
	{
		return Slice<TYPE>(BasePointer(),Size(),sliceOffset,numSliceItems);
	}

	////------------------------------------------------------------------------------
	//template<typename TYPE> size_t
	//	ArrayPartition<TYPE>::Size() const {
	//	return this->num;
	//}

	////------------------------------------------------------------------------------
	//template<typename TYPE>
	//size_t ArrayPartition<TYPE>::Offset() const {
	//	return this->offset;
	//}

	//template<typename TYPE>
	//inline TYPE* ArrayPartition<TYPE>::BasePointer()
	//{
	//	return basePtr;
	//}

	//template<typename TYPE>
	//inline const TYPE* ArrayPartition<TYPE>::BasePointer() const
	//{
	//	return basePtr;
	//}

	template<typename TYPE>
	inline void iSmartArrayPartition<TYPE>::EraseRange(size_t index, size_t num)
	{
		Base()->EraseRange(Offset() + index, num);
		ChangeSize(-(int64_t)num);
	}

	template<typename TYPE>
	inline void iSmartArrayPartition<TYPE>::ShiftRange(size_t StartIndex, size_t numElements, const int64_t& shiftAmmount)
	{
		Base()->ShiftRange(Offset() + StartIndex, numElements, shiftAmmount);
	}

	template<typename TYPE>
	inline void iSmartArrayPartition<TYPE>::insertBlank(const size_t& index, size_t count)
	{
		Base()->insertBlank(Offset() + index, count);
		ChangeSize(count);
	}

	//------------------------------------------------------------------------------
	template<typename TYPE> TYPE*
		iSmartArrayPartition<TYPE>::begin(const int64_t& poffset) {
		if (BasePointer()) {
			return BasePointer() + Offset() + poffset;
		}
		else {
			return nullptr;
		}
	}

	//------------------------------------------------------------------------------
	template<typename TYPE> const TYPE*
		iSmartArrayPartition<TYPE>::begin(const int64_t& poffset) const {
		if (BasePointer()) {
			return BasePointer() + Offset() + poffset;
		}
		else {
			return nullptr;
		}
	}

	//------------------------------------------------------------------------------
	template<typename TYPE> TYPE*
		iSmartArrayPartition<TYPE>::end(const int64_t& poffset) {
		if (BasePointer()) {
			return begin(poffset) + Size();
		}
		else {
			return nullptr;
		}
	}

	//------------------------------------------------------------------------------
	template<typename TYPE> const TYPE*
		iSmartArrayPartition<TYPE>::end(const int64_t& poffset) const {
		if (BasePointer()) {
			return begin(poffset) + Size();
		}
		else {
			return nullptr;
		}
	}

	//} // namespace Oryol

	template<typename TYPE>
	inline TYPE& iSmartArrayPartition<TYPE>::PushBack(const TYPE& elm)
	{
		Base()->Insert(Offset() + Size(), elm);
		ChangeSize(1);
		return Back();
	}

	template<typename TYPE>
	inline TYPE& iSmartArrayPartition<TYPE>::PushBack(TYPE&& elm)
	{
		Base()->Insert(Offset() + Size(), std::move(elm));
		ChangeSize(1);
		return Back();
	}

	template<typename TYPE>
	inline TYPE& iSmartArrayPartition<TYPE>::PushFront(const TYPE& elm)
	{
		Base()->Insert(Offset(), elm);
		ChangeSize(1);
		return Front();
	}

	template<typename TYPE>
	inline TYPE& iSmartArrayPartition<TYPE>::PushFront(TYPE&& elm)
	{
		Base()->Insert(Offset(), std::move(elm));
		ChangeSize(1);
		return Front();
	}

	template<typename TYPE>
	inline void iSmartArrayPartition<TYPE>::Insert(size_t index, const TYPE& elm)
	{
		Base()->Insert(Offset()+index, elm);
		ChangeSize(1);
	}

	template<typename TYPE>
	inline void iSmartArrayPartition<TYPE>::Insert(size_t index, TYPE&& elm)
	{
		Base()->Insert(Offset() + index, std::move(elm));
		ChangeSize(1);
	}

	template<typename TYPE>
	inline TYPE iSmartArrayPartition<TYPE>::PopBack()
	{
		TYPE b = Back();
		Erase(Size() - 1);
		return b;
	}

	template<typename TYPE>
	inline TYPE iSmartArrayPartition<TYPE>::PopBack(size_t numElements)
	{
		//TODO::
		return TYPE();
	}

	template<typename TYPE>
	inline TYPE iSmartArrayPartition<TYPE>::PopFront()
	{
		TYPE f = Front();
		Erase(0);
		return f;
	}

	template<typename TYPE>
	inline TYPE iSmartArrayPartition<TYPE>::PopFront(size_t numElements)
	{
		//TODO::
		return TYPE();
	}
