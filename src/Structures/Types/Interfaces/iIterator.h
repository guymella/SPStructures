#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iIterable
    @ingroup Interfaces
    @brief interface for structures that can be Iterated
*/

//#include <iterator>
#include "Structures/Types.h"

template <typename TYPE>
class iIteratorBase {
public:
	virtual TYPE* Ptr() = 0;
	virtual const TYPE* Ptr() const = 0;
	virtual TYPE& operator*() { return *Ptr(); };
	virtual TYPE& operator->() { return *Ptr(); };
	virtual const TYPE& operator*() const { return *Ptr(); };
	virtual const TYPE& operator->() const { return *Ptr(); };
};

template <typename TYPE>
class iIteratorForward {
public:	
	virtual bool operator+=(int64_t offset) = 0;
};

template <typename TYPE>
class iIteratorBackward {
public:
	virtual bool operator-=(int64_t offset) = 0;
};



template <typename TYPE>
class iIteratorRandom {
public:
	virtual TYPE& operator[](size_t index) = 0;
	virtual const TYPE& operator[](size_t index) const = 0;
};


template <typename TYPE>
class itrFwd : public iIteratorBase<TYPE>, public iIteratorForward<TYPE>  {
public:
	virtual itrFwd<TYPE>* MakeCopyFwd() const = 0;
};

template <typename TYPE>
class itrBwd : public itrFwd<TYPE>, public iIteratorBackward<TYPE> {
public:
	virtual itrFwd<TYPE>* MakeCopyFwd() const override { return MakeCopyBwd(); };
	virtual itrBwd<TYPE>* MakeCopyBwd() const = 0;
};

template <typename TYPE>
class itr : public itrBwd<TYPE>, public iIteratorRandom<TYPE> {
public:
	virtual TYPE& operator[](size_t index) override;
	virtual const TYPE& operator[](size_t index) const override;
	virtual itrBwd<TYPE>* MakeCopyBwd() const override { return MakeCopy(); };
	virtual itr<TYPE>* MakeCopy() const =0;
};

template<typename TYPE>
inline TYPE& itr<TYPE>::operator[](size_t index)
{
	itr<TYPE>* tmp = MakeCopy();
	*tmp += index;
	TYPE* rtn = (*tmp).Ptr();
	std::free(tmp);
	return *rtn;
}

template<typename TYPE>
inline const TYPE& itr<TYPE>::operator[](size_t index) const
{
	itr<TYPE>* tmp = MakeCopy();
	*tmp += index;
	const TYPE* rtn = (*tmp).Ptr();
	std::free(tmp);
	return *rtn;
}
