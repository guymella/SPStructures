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
#include "Structures/Containers/Slice.h"
#include "SmartArrayPartition.h"

//namespace Oryol {

template<typename TYPE>
class iSmartSlice : public iSmartArrayPartition<TYPE>, public iSliceEdit<TYPE> {
	SmartSlice<TYPE> MakeSmartSlice(size_t sliceOffset = 0, size_t numSliceItems = std::numeric_limits<size_t>::max()) override;
};

template<typename TYPE> 
class SmartSlice : public iSmartSlice<TYPE> {
public:
    /// default constructor
	SmartSlice();
    /// init from base pointer, start index and number of items
	SmartSlice(iDArray* base, size_t sliceOffset=0, size_t numSliceItems= std::numeric_limits<size_t>::max());
    /// copy constructor
	SmartSlice(const SmartSlice& rhs);
    /// copy-assignment
    void operator=(const SmartSlice& rhs);
	/// copy-assignment
	bool operator==(const SmartSlice& rhs);
    ///// read/write access to indexed item
    //TYPE& operator[](size_t index) override;
    ///// read-only access to indexed item
    //const TYPE& operator[](size_t index) const override;
    /// create a new slice from this slice
    //Slice<TYPE> MakeSlice(size_t sliceOffset=0, size_t numSliceItems= std::numeric_limits<size_t>::max()) override;
	
		/// get the start index
	virtual size_t Offset() const override;
	//Get the base pointer
	virtual TYPE* BasePointer() override;
	virtual const TYPE* BasePointer() const override;
	virtual iDArray<TYPE>* Base() override;
	virtual const iDArray<TYPE>* Base() const override;
	virtual size_t BaseSize() const override;
    /// reset the slice to its default state
    void Reset();
    /// return true if Slice is empty
    //bool Empty() const;
    /// get number of items
    size_t Size() const override;
    /// set new absolute size
    void SetSize(size_t numSliceItems);
    /// set new absolute offset
    void SetOffset(size_t sliceOffset);
    /// move offset
    void Move(int64_t delta);
    /// if slice is 'to the right' of the gap, move offset to left by gapSize
    void FillGap(int64_t gapOffset, int64_t gapSize);

	SmartSlice<TYPE>& operator++();
	SmartSlice<TYPE>  operator++(int);
	SmartSlice<TYPE>&  operator--();
	SmartSlice<TYPE>  operator--(int);
	SmartSlice<TYPE>& operator+=(int64_t);
	SmartSlice<TYPE>&  operator-=(int64_t);
	SmartSlice<TYPE> operator+(int64_t);
	SmartSlice<TYPE> operator-(int64_t);

	//size_t FindIndexLinear(const TYPE& elm, size_t startIndex, size_t endIndex) const override;

    /*/// C++ begin
    TYPE* begin(const int64_t& offset = 0) override;
    /// C++ begin
    const TYPE* begin(const int64_t& offset = 0) const override;
    /// C++ end
    TYPE* end(const int64_t& offset = 0) override;
    /// C++ end
    const TYPE* end(const int64_t& offset = 0) const override;*/

private:
    iDArray* basePtr = nullptr;
    size_t offset = 0;
    size_t num = 0;
};

//------------------------------------------------------------------------------
template<typename TYPE>
SmartSlice<TYPE>::SmartSlice() {
    // empty
}

//------------------------------------------------------------------------------
template<typename TYPE>
SmartSlice<TYPE>::SmartSlice(iDArray* base, size_t sliceOffset, size_t sliceNumItems):
basePtr(base),
offset(sliceOffset),
num((sliceOffset+sliceNumItems> base->Size())? base->Size()- sliceOffset :sliceNumItems)
{
   // o_assert_dbg(basePtr && (offset>=0) && (num>=0) && ((offset+num)<=baseSize));
}

//------------------------------------------------------------------------------
template<typename TYPE>
SmartSlice<TYPE>::SmartSlice(const SmartSlice& rhs):
basePtr(rhs.basePtr), baseSize(rhs.baseSize), offset(rhs.offset), num(rhs.num) {
    // empty
}

//------------------------------------------------------------------------------
template<typename TYPE> void
SmartSlice<TYPE>::operator=(const SmartSlice& rhs) {
    this->basePtr = rhs.basePtr;
    this->baseSize = rhs.baseSize;
    this->offset = rhs.offset;
    this->num = rhs.num;
}

template<typename TYPE>
inline bool SmartSlice<TYPE>::operator==(const SmartSlice& rhs)
{
	return (basePtr == rhs.basePtr && baseSize == rhs.baseSize && num == rhs.num && offset == rhs.offset);
}

////------------------------------------------------------------------------------
//template<typename TYPE> TYPE&
//SmartSlice<TYPE>::operator[](size_t index) {
//    //o_assert_dbg(this->basePtr && (index >= 0) && (index < this->num));
//    return this->basePtr[this->offset + index];
//}
//
////------------------------------------------------------------------------------
//template<typename TYPE> const TYPE&
//SmartSlice<TYPE>::operator[](size_t index) const {
//    //o_assert_dbg(this->basePtr && (index >= 0) && (index < this->num));
//    return this->basePtr[this->offset + index];
//}

//------------------------------------------------------------------------------
template<typename TYPE> void
SmartSlice<TYPE>::Reset() {
    this->basePtr = nullptr;
    this->offset = 0;
    this->num = 0;
}

//------------------------------------------------------------------------------
//template<typename TYPE> bool
//Slice<TYPE>::Empty() const {
//    return 0 == this->num;
//}



//------------------------------------------------------------------------------
template<typename TYPE> void
SmartSlice<TYPE>::SetSize(size_t numSliceItems) {
    //o_assert_dbg((numSliceItems >= 0) && ((this->offset+numSliceItems) <= this->baseSize));
    this->num = numSliceItems;
}

//------------------------------------------------------------------------------
template<typename TYPE> 
size_t SmartSlice<TYPE>::Offset() const {
    return this->offset;
}

//------------------------------------------------------------------------------
template<typename TYPE> 
void SmartSlice<TYPE>::SetOffset(size_t sliceOffset) {
    //o_assert_dbg((sliceOffset >= 0) && ((sliceOffset+this->num) <= this->baseSize));
    this->offset = sliceOffset;
}

//------------------------------------------------------------------------------
template<typename TYPE> 
void SmartSlice<TYPE>::Move(int64_t delta) {
    //o_assert_dbg((this->offset + delta) >= 0);
    //o_assert_dbg((this->offset + delta + this->num) <= this->baseSize);
    this->offset += delta;
}

//------------------------------------------------------------------------------
template<typename TYPE> void
SmartSlice<TYPE>::FillGap(int64_t gapOffset, int64_t gapSize) {
    //o_assert((gapOffset >= 0) && (gapSize > 0) && ((gapOffset+gapSize) <= this->baseSize));
    if (this->offset >= (gapOffset + gapSize)) {
        this->offset -= gapSize;
    }
}

template<typename TYPE>
inline SmartSlice<TYPE>& SmartSlice<TYPE>::operator++()
{
	Move(1);
	return *this;
}

template<typename TYPE>
inline SmartSlice<TYPE> SmartSlice<TYPE>::operator++(int)
{
	Slice<TYPE> t = *this;
	Move(1);
	return t;
}

template<typename TYPE>
inline SmartSlice<TYPE>& SmartSlice<TYPE>::operator--()
{
	Move(-1);
	return *this;
}

template<typename TYPE>
inline SmartSlice<TYPE> SmartSlice<TYPE>::operator--(int)
{
	Slice<TYPE> t = *this;
	Move(-1);
	return t;
}

template<typename TYPE>
inline SmartSlice<TYPE>& SmartSlice<TYPE>::operator+=(int64_t o)
{
	Move(o);
	return *this;
}

template<typename TYPE>
inline SmartSlice<TYPE>& SmartSlice<TYPE>::operator-=(int64_t o)
{
	Move(-o);
	return *this;
}

template<typename TYPE>
inline SmartSlice<TYPE> SmartSlice<TYPE>::operator+(int64_t o)
{
	Slice<TYPE> t = *this;
	t.Move(o);
	return t;
}

template<typename TYPE>
inline SmartSlice<TYPE> SmartSlice<TYPE>::operator-(int64_t o)
{
	Slice<TYPE> t = *this;
	t.Move(-o);
	return t;
}



//} // namespace Oryol
