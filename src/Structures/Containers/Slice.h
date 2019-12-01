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

//namespace Oryol {
template<typename TYPE> class Slice;

template<typename TYPE> 
class iSliceable {
public:
	virtual Slice<TYPE> MakeSlice(size_t sliceOffset = 0, size_t numSliceItems = std::numeric_limits<size_t>::max()) = 0;
};


template<typename TYPE> 
class Slice : public iArray<TYPE>, public iSliceable<TYPE> {
public:
    /// default constructor
    Slice();
    /// init from base pointer, start index and number of items
    Slice(TYPE* base, size_t numBaseItems, size_t sliceOffset=0, size_t numSliceItems= std::numeric_limits<size_t>::max());
    /// copy constructor
    Slice(const Slice& rhs);
    /// copy-assignment
    void operator=(const Slice& rhs);
    /// read/write access to indexed item
    TYPE& operator[](size_t index) override;
    /// read-only access to indexed item
    const TYPE& operator[](size_t index) const override;
    /// create a new slice from this slice
    Slice<TYPE> MakeSlice(size_t sliceOffset=0, size_t numSliceItems= std::numeric_limits<size_t>::max()) override;

    /// reset the slice to its default state
    void Reset();
    /// return true if Slice is empty
    //bool Empty() const;
    /// get number of items
    size_t Size() const override;
    /// set new absolute size
    void SetSize(size_t numSliceItems);
    /// get the start index
    size_t Offset() const;
    /// set new absolute offset
    void SetOffset(size_t sliceOffset);
    /// move offset
    void Move(int64_t delta);
    /// if slice is 'to the right' of the gap, move offset to left by gapSize
    void FillGap(int64_t gapOffset, int64_t gapSize);

	//size_t FindIndexLinear(const TYPE& elm, size_t startIndex, size_t endIndex) const override;

    /// C++ begin
    TYPE* begin(const int64_t& offset = 0) override;
    /// C++ begin
    const TYPE* begin(const int64_t& offset = 0) const override;
    /// C++ end
    TYPE* end(const int64_t& offset = 0) override;
    /// C++ end
    const TYPE* end(const int64_t& offset = 0) const override;

private:
    TYPE* basePtr = nullptr;
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

//------------------------------------------------------------------------------
template<typename TYPE> TYPE&
Slice<TYPE>::operator[](size_t index) {
    //o_assert_dbg(this->basePtr && (index >= 0) && (index < this->num));
    return this->basePtr[this->offset + index];
}

//------------------------------------------------------------------------------
template<typename TYPE> const TYPE&
Slice<TYPE>::operator[](size_t index) const {
    //o_assert_dbg(this->basePtr && (index >= 0) && (index < this->num));
    return this->basePtr[this->offset + index];
}

//------------------------------------------------------------------------------
template<typename TYPE> Slice<TYPE>
Slice<TYPE>::MakeSlice(size_t sliceOffset, size_t numSliceItems) {
    if (sliceOffset+numSliceItems > Size()) {
        numSliceItems = Size() - sliceOffset;
    }
    return Slice(this->basePtr, this->baseSize, this->offset+sliceOffset, numSliceItems);
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
//template<typename TYPE> bool
//Slice<TYPE>::Empty() const {
//    return 0 == this->num;
//}

//------------------------------------------------------------------------------
template<typename TYPE> size_t
Slice<TYPE>::Size() const {
    return this->num;
}

//------------------------------------------------------------------------------
template<typename TYPE> void
Slice<TYPE>::SetSize(size_t numSliceItems) {
    o_assert_dbg((numSliceItems >= 0) && ((this->offset+numSliceItems) <= this->baseSize));
    this->num = numSliceItems;
}

//------------------------------------------------------------------------------
template<typename TYPE> 
size_t Slice<TYPE>::Offset() const {
    return this->offset;
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
template<typename TYPE> void
Slice<TYPE>::FillGap(int64_t gapOffset, int64_t gapSize) {
    //o_assert((gapOffset >= 0) && (gapSize > 0) && ((gapOffset+gapSize) <= this->baseSize));
    if (this->offset >= (gapOffset + gapSize)) {
        this->offset -= gapSize;
    }
}

//------------------------------------------------------------------------------
template<typename TYPE> TYPE*
Slice<TYPE>::begin(const int64_t& poffset) {
    if (this->basePtr) {
        return this->basePtr + this->offset + poffset;
    }
    else {
        return nullptr;
    }
}

//------------------------------------------------------------------------------
template<typename TYPE> const TYPE*
Slice<TYPE>::begin(const int64_t& poffset) const {
    if (this->basePtr) {
        return this->basePtr + this->offset + poffset;
    }
    else {
        return nullptr;
    }
}

//------------------------------------------------------------------------------
template<typename TYPE> TYPE*
Slice<TYPE>::end(const int64_t& poffset) {
    if (this->basePtr) {
        return this->basePtr + this->offset + this->num + poffset;
    }
    else {
        return nullptr;
    }
}

//------------------------------------------------------------------------------
template<typename TYPE> const TYPE*
Slice<TYPE>::end(const int64_t& poffset) const {
    if (this->basePtr) {
        return this->basePtr + this->offset + this->num + poffset;
    }
    else {
        return nullptr;
    }
}

//} // namespace Oryol
