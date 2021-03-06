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
#include "Interfaces/iArray.h"

//namespace Oryol {
template<typename TYPE> class Slice;

template<typename TYPE> 
class iSliceable {
public:
	virtual Slice<TYPE> MakeSlice(size_t sliceOffset = 0, size_t numSliceItems = std::numeric_limits<size_t>::max()) = 0;
};

template<typename TYPE>
class iArrayPartition : public iArray<TYPE>, public iSliceable<TYPE> {	
public:
	/// read/write access to indexed item
	//TYPE& operator[](size_t index) override;
	/// read-only access to indexed item
	//const TYPE& operator[](size_t index) const override;
	/// create a new slice from this slice
	Slice<TYPE> MakeSlice(size_t sliceOffset = 0, size_t numSliceItems = std::numeric_limits<size_t>::max()) override;
	/// get number of items
	//size_t Size() const overrid;
	/// get the start index
	virtual size_t Offset() const = 0;
	//Get the base pointer
	virtual TYPE* BasePointer() = 0;
	virtual const TYPE* BasePointer() const= 0;
	//size_t FindIndexLinear(const TYPE& elm, size_t startIndex, size_t endIndex) const override;

	/// C++ begin
	virtual Itr<TYPE> begin(const int64_t& offset = 0) override;
	/// C++ begin
	virtual Itr<const TYPE> begin(const int64_t& offset = 0) const override;
	/// C++ end
	virtual Itr<TYPE> end(const int64_t& offset = 0) override;
	/// C++ end
	virtual Itr<const TYPE> end(const int64_t& offset = 0) const override;
};

template<typename TYPE>
class ArrayPartition : public iArrayPartition<TYPE> {
public:
	/// default constructor
	ArrayPartition();
	/// init from base pointer, start index and number of items
	ArrayPartition(TYPE* base, size_t numBaseItems, size_t sliceOffset = 0, size_t numSliceItems = std::numeric_limits<size_t>::max());
	/// copy constructor
	ArrayPartition(const ArrayPartition<TYPE>& rhs);
	/// Equality
	bool operator==(const ArrayPartition<TYPE>& rhs);
	/// copy-assignment
	void operator=(const ArrayPartition<TYPE>& rhs);
	/// get number of items
	size_t Size() const override;
	/// get the start index
	virtual size_t Offset() const override;
	//Get the base pointer
	virtual TYPE* BasePointer() override;
	virtual const TYPE* BasePointer() const override;

protected:
	TYPE* basePtr = nullptr;
	//size_t baseSize = 0;
	size_t offset = 0;
	size_t num = 0;
};




template<typename TYPE>
inline ArrayPartition<TYPE>::ArrayPartition()
{
}

//------------------------------------------------------------------------------
template<typename TYPE>
ArrayPartition<TYPE>::ArrayPartition(TYPE* base, size_t numBaseItems, size_t sliceOffset, size_t sliceNumItems):
basePtr(base),
offset(sliceOffset),
num((sliceOffset+sliceNumItems> numBaseItems)?numBaseItems:sliceNumItems)
{
   // o_assert_dbg(basePtr && (offset>=0) && (num>=0) && ((offset+num)<=baseSize));
}

template<typename TYPE>
inline ArrayPartition<TYPE>::ArrayPartition(const ArrayPartition<TYPE>& rhs) 
	: basePtr(rhs.basePtr), offset(rhs.offset), num(rhs.num)
{
}

//------------------------------------------------------------------------------
template<typename TYPE> void
ArrayPartition<TYPE>::operator=(const ArrayPartition<TYPE>& rhs) {
    this->basePtr = rhs.basePtr;
    this->baseSize = rhs.baseSize;
    this->offset = rhs.offset;
    this->num = rhs.num;
}

template<typename TYPE>
inline bool ArrayPartition<TYPE>::operator==(const ArrayPartition<TYPE>& rhs)
{
	return (basePtr == rhs.basePtr && baseSize == rhs.baseSize && num == rhs.num && offset == rhs.offset);
}

////------------------------------------------------------------------------------
//template<typename TYPE> TYPE&
//iArrayPartition<TYPE>::operator[](size_t index) {
//    //o_assert_dbg(this->basePtr && (index >= 0) && (index < this->num));
//    return begin()[index];
//}
//
////------------------------------------------------------------------------------
//template<typename TYPE> const TYPE&
//iArrayPartition<TYPE>::operator[](size_t index) const {
//    //o_assert_dbg(this->basePtr && (index >= 0) && (index < this->num));
//    return begin()[index];
//}

//------------------------------------------------------------------------------
template<typename TYPE> Slice<TYPE>
iArrayPartition<TYPE>::MakeSlice(size_t sliceOffset, size_t numSliceItems) {
    if (sliceOffset+numSliceItems > Size()) {
        numSliceItems = Size() - sliceOffset;
    }
    return Slice<TYPE>(BasePointer(), Size(), Offset()+sliceOffset, numSliceItems);
}

//------------------------------------------------------------------------------
template<typename TYPE> size_t
ArrayPartition<TYPE>::Size() const {
    return this->num;
}

//------------------------------------------------------------------------------
template<typename TYPE> 
size_t ArrayPartition<TYPE>::Offset() const {
    return this->offset;
}

template<typename TYPE>
inline TYPE* ArrayPartition<TYPE>::BasePointer()
{
	return basePtr;
}

template<typename TYPE>
inline const TYPE* ArrayPartition<TYPE>::BasePointer() const
{
	return basePtr;
}

//------------------------------------------------------------------------------
template<typename TYPE> 
Itr<TYPE> iArrayPartition<TYPE>::begin(const int64_t& poffset) {
    if (BasePointer()) {
        return Itr<TYPE>(BasePointer() + Offset() + poffset);
    }
    else {
        return Itr<TYPE>(nullptr);
    }
}

//------------------------------------------------------------------------------
template<typename TYPE> 
Itr<const TYPE> iArrayPartition<TYPE>::begin(const int64_t& poffset) const {
	if (BasePointer()) {
		return Itr<const TYPE>(BasePointer() + Offset() + poffset);
	}
	else {
		return Itr<const TYPE>(nullptr);
	}
}

//------------------------------------------------------------------------------
template<typename TYPE>
Itr<TYPE> iArrayPartition<TYPE>::end(const int64_t& poffset) {
    if (BasePointer()) {
        return begin(poffset+ Size());
    }
    else {
        return Itr<TYPE>(nullptr);
    }
}

//------------------------------------------------------------------------------
template<typename TYPE>
Itr<const TYPE> iArrayPartition<TYPE>::end(const int64_t& poffset) const {
	if (BasePointer()) {
		return begin(poffset + Size());
	}
	else {
		return Itr<const TYPE>(nullptr);
	}
}

//} // namespace Oryol
