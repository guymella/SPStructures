#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iArray
    @ingroup Interfaces
    @brief interface for Procedural Memory inline Array
*/

#include "iDynamic.h"
#include "iCountable.h"
#include "iIndexable.h"
#include "iIterable.h"
#include "iQueue.h"

template <typename TYPE>
class iArray : public iCountable, public iIndexable<TYPE>, public iCIterable<TYPE>, public iSearchable<TYPE> {
public:
	virtual size_t FindIndexLinear(const TYPE& elm, size_t startIndex = 0, size_t endIndex = std::numeric_limits<size_t>::max()) const override;
	/// read/write access to indexed item
	TYPE& operator[](size_t index) override;
	/// read-only access to indexed item
	const TYPE& operator[](size_t index) const override;				
};

template <typename TYPE>
class iDArray : public iArray<TYPE>, public iEditable<TYPE>, public iQueueDouble<TYPE> {
public:

	virtual TYPE& Front() override;
	/// read-only access to first element (must exist)
	virtual const TYPE& Front() const override;
	/// read/write access to last element (must exist)
	virtual TYPE& Back() override;
	/// read-only access to last element (must exist)
	virtual const TYPE& Back() const override;

};

template <typename TYPE>
class iDDArray : public iDArray<TYPE>, public iDynamic {
public:

	
};

template<class TYPE> 
size_t iArray<TYPE>::FindIndexLinear(const TYPE& elm, size_t startIndex, size_t endIndex) const {
	if (!Empty()) {
		if (endIndex > Size())
			endIndex = Size();

		const TYPE* bgn = begin();
		for (size_t i = startIndex; i < endIndex; i++) {
			if (elm == bgn[i]) {
				return i;
			}
		}
	}
	// fallthrough: not found
	return 0;
}

//------------------------------------------------------------------------------
template<typename TYPE> TYPE&
iArray<TYPE>::operator[](size_t index) {
	//o_assert_dbg(this->basePtr && (index >= 0) && (index < this->num));
	return begin()[index];
}

//------------------------------------------------------------------------------
template<typename TYPE> const TYPE&
iArray<TYPE>::operator[](size_t index) const {
	//o_assert_dbg(this->basePtr && (index >= 0) && (index < this->num));
	return begin()[index];
}

//------------------------------------------------------------------------------
template<class TYPE> TYPE&
iDArray<TYPE>::Front() {
	//TODO:: handle empty
	return *begin();
}

//------------------------------------------------------------------------------
template<class TYPE> const TYPE&
iDArray<TYPE>::Front() const {
	//TODO:: handle empty
	return *begin();
}

//------------------------------------------------------------------------------
template<class TYPE> TYPE&
iDArray<TYPE>::Back() {
	//TODO:: handle empty
	TYPE* e = end();
	return *(--e);
}

//------------------------------------------------------------------------------
template<class TYPE> const TYPE&
iDArray<TYPE>::Back() const {
	//TODO:: handle empty
	const TYPE* e = end();
	return *(--e);
}