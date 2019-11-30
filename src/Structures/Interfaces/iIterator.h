#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iIterable
    @ingroup Interfaces
    @brief interface for structures that can be Iterated
*/

#include <iterator>

template <typename TYPE>
class iIteratorBase {
public:
	//virtual iIteratorBase(const iIteratorBase<TYPE>& rhs) = 0;
	virtual void operator=(const iIteratorBase<TYPE>& rhs) = 0;
	virtual TYPE& operator*() = 0;
	virtual TYPE& operator->() = 0;
};

template <typename TYPE>
class iIteratorForward {
public:	
	virtual iIteratorForward<TYPE>& operator++() = 0;
	virtual iIteratorForward<TYPE> operator++(int) = 0;
};

template <typename TYPE>
class iIteratorBackward {
public:
	virtual iIteratorBackward<TYPE>& operator--() = 0;
	virtual iIteratorBackward<TYPE> operator--(int) = 0;
};



template <typename TYPE>
class iIteratorRandom {
public:
	
	virtual iIteratorRandom<TYPE>& operator+=(size_t) = 0;
	virtual iIteratorRandom<TYPE> operator+(size_t offset) = 0;
	virtual iIteratorRandom<TYPE>& operator-=(size_t) = 0;
	virtual iIteratorRandom<TYPE> operator-(size_t offset) = 0;
	virtual iIteratorRandom<TYPE> operator-(const iIteratorRandom<TYPE>& itr) = 0;
	virtual TYPE& operator[](size_t index) = 0;
};


template <typename TYPE>
class ItrFwd : public iIteratorBase<TYPE>, public iIteratorForward<TYPE>  {
public:
};

template <typename TYPE>
class ItrBdr : public ItrFwd<TYPE>, public iIteratorBackward<TYPE> {
public:
};

template <typename TYPE>
class Itr : public ItrBdr<TYPE>, public iIteratorRandom<TYPE> {
public:
};