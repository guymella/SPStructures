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
#include "ArrayPartition.h"

template<typename TYPE>
class iSlice : public iArrayPartition<TYPE> {
public:
	virtual size_t BaseSize() const = 0;
	/// create a new slice from this slice
	virtual Slice<TYPE> MakeSlice(size_t sliceOffset = 0, size_t numSliceItems = std::numeric_limits<size_t>::max()) override;
	/// reset the slice to its default state
	virtual void Reset() = 0;
	/// set new absolute size
	virtual void SetSize(size_t numSliceItems) = 0;
	/// set new absolute offset
	virtual void SetOffset(size_t sliceOffset) = 0;
	/// move offset
	virtual void Move(int64_t delta) = 0;
	/// if slice is 'to the right' of the gap, move offset to left by gapSize
	//virtual void FillGap(int64_t gapOffset, int64_t gapSize) = 0;

	virtual Slice<TYPE>& operator++() = 0;
	virtual Slice<TYPE>  operator++(int) = 0;
	virtual Slice<TYPE>& operator--() = 0;
	virtual Slice<TYPE>  operator--(int) = 0;
	virtual Slice<TYPE>& operator+=(int64_t) = 0;
	virtual Slice<TYPE>& operator-=(int64_t) = 0;
	virtual Slice<TYPE> operator+(int64_t) = 0;
	virtual Slice<TYPE> operator-(int64_t) = 0;
};


template<typename TYPE> 
class Slice : public iSlice<TYPE> {
public:
    /// default constructor
    Slice();
    /// init from base pointer, start index and number of items
    Slice(TYPE* base, size_t numBaseItems, size_t sliceOffset=0, size_t numSliceItems= std::numeric_limits<size_t>::max());
    /// copy constructor
    Slice(const Slice& rhs);
    /// copy-assignment
    void operator=(const Slice& rhs);
	/// copy-assignment
	bool operator==(const Slice& rhs);
    

	/// get number of items
	virtual size_t Size() const override;
	/// get the start index
	virtual size_t Offset() const override;
	//Get the base pointer
	TYPE* BasePointer() override;
	const TYPE* BasePointer() const override;
	size_t BaseSize() const override;

    /// reset the slice to its default state
    void Reset() override;
    /// set new absolute size
    void SetSize(size_t numSliceItems) override;
    /// set new absolute offset
    void SetOffset(size_t sliceOffset) override;
    /// move offset
    void Move(int64_t delta) override;
    /// if slice is 'to the right' of the gap, move offset to left by gapSize
    //void FillGap(int64_t gapOffset, int64_t gapSize) override;

	Slice<TYPE>& operator++() override;
	Slice<TYPE>  operator++(int) override;
	Slice<TYPE>&  operator--() override;
	Slice<TYPE>  operator--(int) override;
	Slice<TYPE>& operator+=(int64_t) override;
	Slice<TYPE>&  operator-=(int64_t) override;
	Slice<TYPE> operator+(int64_t) override;
	Slice<TYPE> operator-(int64_t) override;




private:
    TYPE *basePtr =0;
    size_t baseSize = 0;
	size_t offset = 0;
	size_t num = 0;
};

//------------------------------------------------------------------------------
template<typename TYPE>
Slice<TYPE>::Slice() {
    // empty
}

//------------------------------------------------------------------------------
template<typename TYPE>
Slice<TYPE>::Slice(TYPE* base, size_t numBaseItems, size_t sliceOffset, size_t sliceNumItems):
basePtr(base),
baseSize(numBaseItems),
offset(sliceOffset),
num((sliceOffset+sliceNumItems> numBaseItems)?numBaseItems:sliceNumItems)
{
   // o_assert_dbg(basePtr && (offset>=0) && (num>=0) && ((offset+num)<=baseSize));
}

//------------------------------------------------------------------------------
template<typename TYPE>
Slice<TYPE>::Slice(const Slice& rhs):
basePtr(rhs.basePtr), baseSize(rhs.baseSize), offset(rhs.offset), num(rhs.num) {
    // empty
}

//------------------------------------------------------------------------------
template<typename TYPE> void
Slice<TYPE>::operator=(const Slice& rhs) {
    this->basePtr = rhs.basePtr;
    this->baseSize = rhs.baseSize;
    this->offset = rhs.offset;
    this->num = rhs.num;
}

template<typename TYPE>
inline bool Slice<TYPE>::operator==(const Slice& rhs)
{
	return (basePtr == rhs.basePtr && baseSize == rhs.baseSize && num == rhs.num && offset == rhs.offset);
}


//------------------------------------------------------------------------------
template<typename TYPE> 
size_t Slice<TYPE>::Size() const {
	return this->num;
}


//------------------------------------------------------------------------------
template<typename TYPE> void
Slice<TYPE>::Reset() {
    this->basePtr = nullptr;
    this->baseSize = 0;
    this->offset = 0;
    this->num = 0;
}

//------------------------------------------------------------------------------
template<typename TYPE> void
Slice<TYPE>::SetSize(size_t numSliceItems) {
    //o_assert_dbg((numSliceItems >= 0) && ((this->offset+numSliceItems) <= this->baseSize));
    this->num = numSliceItems;
}


//------------------------------------------------------------------------------
template<typename TYPE> 
void Slice<TYPE>::SetOffset(size_t sliceOffset) {
    //o_assert_dbg((sliceOffset >= 0) && ((sliceOffset+this->num) <= this->baseSize));
    this->offset = sliceOffset;
}

//------------------------------------------------------------------------------
template<typename TYPE> 
void Slice<TYPE>::Move(int64_t delta) {
    //o_assert_dbg((this->offset + delta) >= 0);
    //o_assert_dbg((this->offset + delta + this->num) <= this->baseSize);
    this->offset += delta;
}

//------------------------------------------------------------------------------
//template<typename TYPE> void
//Slice<TYPE>::FillGap(int64_t gapOffset, int64_t gapSize) {
//    //o_assert((gapOffset >= 0) && (gapSize > 0) && ((gapOffset+gapSize) <= this->baseSize));
//    if (this->offset >= (gapOffset + gapSize)) {
//        this->offset -= gapSize;
//    }
//}

template<typename TYPE>
inline Slice<TYPE>& Slice<TYPE>::operator++()
{
	Move(1);
	return *this;
}

template<typename TYPE>
inline Slice<TYPE> Slice<TYPE>::operator++(int)
{
	Slice<TYPE> t = *this;
	Move(1);
	return t;
}

template<typename TYPE>
inline Slice<TYPE>& Slice<TYPE>::operator--()
{
	Move(-1);
	return *this;
}

template<typename TYPE>
inline Slice<TYPE> Slice<TYPE>::operator--(int)
{
	Slice<TYPE> t = *this;
	Move(-1);
	return t;
}

template<typename TYPE>
inline Slice<TYPE>& Slice<TYPE>::operator+=(int64_t o)
{
	Move(o);
	return *this;
}

template<typename TYPE>
inline Slice<TYPE>& Slice<TYPE>::operator-=(int64_t o)
{
	Move(-o);
	return *this;
}

template<typename TYPE>
inline Slice<TYPE> Slice<TYPE>::operator+(int64_t o)
{
	Slice<TYPE> t = *this;
	t.Move(o);
	return t;
}

template<typename TYPE>
inline Slice<TYPE> Slice<TYPE>::operator-(int64_t o)
{
	Slice<TYPE> t = *this;
	t.Move(-o);
	return t;
}



//} // namespace Oryol

template<typename TYPE>
inline Slice<TYPE> iSlice<TYPE>::MakeSlice(size_t sliceOffset, size_t numSliceItems)
{
	return Slice<TYPE>(BasePointer(),BaseSize(),Offset()+sliceOffset,numSliceItems);
}

template<typename TYPE>
inline size_t Slice<TYPE>::Offset() const
{
	return offset;
}

template<typename TYPE>
inline TYPE* Slice<TYPE>::BasePointer()
{
	return basePtr;
}

template<typename TYPE>
inline const TYPE* Slice<TYPE>::BasePointer() const
{
	return basePtr;
}

template<typename TYPE>
inline size_t Slice<TYPE>::BaseSize() const
{
	return baseSize;
}
