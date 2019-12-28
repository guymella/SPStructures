//------------------------------------------------------------------------------
//  VarInt.h
//------------------------------------------------------------------------------

#include "Interfaces/iIterator.h"
#include <memory>

template <typename TYPE, typename itrTYPE = itr<TYPE>>
class Itr { //an interface adapter for Iterator and pointers
public:
	Itr() {};
	Itr(itrTYPE* Ptr) : i(Ptr), isItr(true) {};
	Itr(TYPE* Ptr) : p(Ptr) , isItr(false) {};
	Itr(const itrTYPE& Itr);
	Itr(itrTYPE&& Itr);
	Itr(const Itr<TYPE>& rhs);
	Itr(Itr<TYPE>&& rhs);
	~Itr() { if (isItr) std::free(i); };
	void operator=(Itr<TYPE>&& rhs);
	void operator=(const Itr<TYPE>& rhs);
	bool operator==(const Itr<TYPE>& rhs) const;
	bool operator!=(const Itr<TYPE>& rhs) const;
	TYPE* Ptr() { return isItr ? i->Ptr() : p; };
	const TYPE* Ptr() const { return isItr ? i->Ptr() : p; };
	TYPE& operator*() { return *Ptr(); };
	TYPE& operator->() { return *Ptr();};
	const TYPE& operator*() const { return *Ptr();};
	const TYPE& operator->() const { return *Ptr();	};
	Itr<TYPE, itrTYPE>& operator++();
	Itr<TYPE, itrTYPE> operator++(int);
	Itr<TYPE, itrTYPE>& operator--();
	Itr<TYPE, itrTYPE> operator--(int);
	Itr<TYPE, itrTYPE>& operator+=(int64_t offset);
	Itr<TYPE, itrTYPE> operator+(int64_t offset) const;
	Itr<TYPE, itrTYPE>& operator-=(int64_t offset);
	Itr<TYPE, itrTYPE> operator-(int64_t offset) const;
	int64_t operator-(const Itr<TYPE>& rhs) const;
	TYPE& operator[](size_t index);
	const TYPE& operator[](size_t index) const;
	operator void* () const { return (void*)Ptr(); };
	operator size_t () const { return (size_t)Ptr(); };
	//operator TYPE* () { return (TYPE*)Ptr(); };
	//operator const TYPE* () const { return (TYPE*)Ptr(); };
	/// Compare from iterators
	static int64_t Compare(Itr<TYPE> a, Itr<TYPE> b, Itr<TYPE> bEnd);
private:
	bool isItr = false;
	union { 
		itrTYPE* i;
		TYPE* p;
	};
};

template<typename TYPE, typename itrTYPE>
inline Itr<TYPE, itrTYPE>::Itr(const itrTYPE& Itr) : isItr(true)
{
	i = Itr.MakeCopy();
}

template<typename TYPE, typename itrTYPE>
inline Itr<TYPE, itrTYPE>::Itr(itrTYPE&& Itr)
{
	i = Itr.MakeCopy();
}

template<typename TYPE, typename itrTYPE>
inline Itr<TYPE, itrTYPE>::Itr(const Itr<TYPE>& rhs)
{
	if (rhs.isItr)
		i = rhs.i->MakeCopy();
	else
		p = rhs.p;
}

template<typename TYPE, typename itrTYPE>
inline Itr<TYPE, itrTYPE>::Itr(Itr<TYPE>&& rhs)
{
	if (rhs.isItr){
		i = rhs.i;
		rhs.i = 0;
		rhs.isItr = false;//dont free on destroy
	}
	else
		p = rhs.p;

}

template<typename TYPE, typename itrTYPE>
inline void Itr<TYPE, itrTYPE>::operator=(Itr<TYPE>&& rhs)
{
	if (rhs.isItr) {
		i = rhs.i;
		rhs.i = 0;
		rhs.isItr = false;//dont free on destroy
	}
	else
		p = rhs.p;
}

template<typename TYPE, typename itrTYPE>
inline void Itr<TYPE, itrTYPE>::operator=(const Itr<TYPE>& rhs)
{
	if (rhs.isItr)
		i = rhs.i->MakeCopy();
	else
		p = rhs.p;	
}

template<typename TYPE, typename itrTYPE>
inline bool Itr<TYPE, itrTYPE>::operator==(const Itr<TYPE>& rhs) const
{
	if (isItr && rhs.isItr)
		return i == rhs.i;
	if (!isItr && !rhs.isItr)
		return p == rhs.p;
	return false;
}

template<typename TYPE, typename itrTYPE>
inline bool Itr<TYPE, itrTYPE>::operator!=(const Itr<TYPE>& rhs) const
{
	return !((*this) == rhs);
}

template<typename TYPE, typename itrTYPE>
inline Itr<TYPE, itrTYPE>& Itr<TYPE, itrTYPE>::operator++()
{
	if (isItr)
		++i;
	else
		++p;
	return *this;
}

template<typename TYPE, typename itrTYPE>
inline Itr<TYPE, itrTYPE> Itr<TYPE, itrTYPE>::operator++(int)
{
	Itr<TYPE> rtn(*this);
	++(*this);
	return rtn;
}

template<typename TYPE, typename itrTYPE>
inline Itr<TYPE, itrTYPE>& Itr<TYPE, itrTYPE>::operator--()
{
	if (isItr) 
		--i;
	else 
		--p;
	return *this;	
}

template<typename TYPE, typename itrTYPE>
inline Itr<TYPE, itrTYPE> Itr<TYPE, itrTYPE>::operator--(int)
{
	Itr<TYPE> rtn(*this);
	--(*this);
	return rtn;
}

template<typename TYPE, typename itrTYPE>
inline Itr<TYPE, itrTYPE>& Itr<TYPE, itrTYPE>::operator+=(int64_t offset)
{
	if (isItr)
		i += offset;
	else
		p += offset;
	return (*this);
}

template<typename TYPE, typename itrTYPE>
inline Itr<TYPE, itrTYPE> Itr<TYPE, itrTYPE>::operator+(int64_t offset) const
{
	ptr<TYPE> rtn(*this);
	rtn += offset;
	return rtn;
}

template<typename TYPE, typename itrTYPE>
inline Itr<TYPE, itrTYPE>& Itr<TYPE, itrTYPE>::operator-=(int64_t offset)
{
	if (isItr)
		i -= offset;
	else
		p -= offset;
	return (*this);
}

template<typename TYPE, typename itrTYPE>
inline Itr<TYPE, itrTYPE> Itr<TYPE, itrTYPE>::operator-(int64_t offset)const
{
	ptr<TYPE> rtn(*this);
	rtn -= offset;
	return rtn;
}

template<typename TYPE, typename itrTYPE>
inline int64_t Itr<TYPE, itrTYPE>::operator-(const Itr<TYPE>& rhs) const
{
	if (isItr && rhs.isItr)
		return std::numeric_limits<int64_t>::max(); // *i - *(rhs.i);
	if (!isItr && !rhs.isItr)
		return p - rhs.p;
			
	return std::numeric_limits<int64_t>::max();
}

template<typename TYPE, typename itrTYPE>
inline TYPE& Itr<TYPE, itrTYPE>::operator[](size_t index)
{
	if (isItr)
		return (*i)[index];
	else
		return p[index];
}

template<typename TYPE, typename itrTYPE>
inline int64_t Itr<TYPE, itrTYPE>::Compare(Itr<TYPE> a, Itr<TYPE> b, Itr<TYPE> bEnd)
{
	int64_t c = 0;
	while (b != bEnd) {
		if (*a == *b)
			c++;
		else if (*(a) < *(b))
			return -c;
		else
			return c;
		a++;
		b++;
	}
	return c;
}
