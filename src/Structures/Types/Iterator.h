//------------------------------------------------------------------------------
//  VarInt.h
//------------------------------------------------------------------------------

#include "Interfaces/iIterator.h"



template <typename TYPE>
class Itr { //an interface adapter for Iterator and pointers
public:
	//ptr(itr<TYPE>* Ptr) : i(Ptr), isItr(true) {};
	Itr(TYPE* Ptr) : p(Ptr) , isItr(false) {};
	Itr(const itr<TYPE>& Itr);
	Itr(itr<TYPE>&& Itr);
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
	Itr<TYPE>& operator++();
	Itr<TYPE> operator++(int);
	Itr<TYPE>& operator--();
	Itr<TYPE> operator--(int);
	Itr<TYPE>& operator+=(int64_t offset);
	Itr<TYPE> operator+(int64_t offset) const;
	Itr<TYPE>& operator-=(int64_t offset);
	Itr<TYPE> operator-(int64_t offset) const;
	int64_t operator-(const Itr<TYPE>& rhs) const;
	TYPE& operator[](size_t index);
	TYPE operator[](size_t index) const;
	operator void* () const { return (void*)Ptr(); };
	operator size_t () const { return (size_t)Ptr(); };
	operator TYPE* () { return (TYPE*)Ptr(); };
	operator const TYPE* () const { return (TYPE*)Ptr(); };
private:
	bool isItr = false;
	union { 
		itr<TYPE>* i;
		TYPE* p;
	};
};

template<typename TYPE>
inline Itr<TYPE>::Itr(const itr<TYPE>& Itr) : isItr(true)
{
	i = Itr.MakeCopy();
}

template<typename TYPE>
inline Itr<TYPE>::Itr(itr<TYPE>&& Itr)
{
	i = Itr.MakeCopy();
}

template<typename TYPE>
inline Itr<TYPE>::Itr(const Itr<TYPE>& rhs)
{
	if (rhs.isItr)
		i = rhs.MakeCopy();
	else
		p = rhs.p;
}

template<typename TYPE>
inline Itr<TYPE>::Itr(Itr<TYPE>&& rhs)
{
	if (rhs.isItr){
		i = rhs.i;
		rhs.i = 0;
		rhs.isItr = false;//dont free on destroy
	}
	else
		p = rhs.p;

}

template<typename TYPE>
inline void Itr<TYPE>::operator=(Itr<TYPE>&& rhs)
{
	if (rhs.isItr) {
		i = rhs.i;
		rhs.i = 0;
		rhs.isItr = false;//dont free on destroy
	}
	else
		p = rhs.p;
}

template<typename TYPE>
inline void Itr<TYPE>::operator=(const Itr<TYPE>& rhs)
{
	if (rhs.isItr)
		i = rhs.MakeCopy();
	else
		p = rhs.p;	
}

template<typename TYPE>
inline bool Itr<TYPE>::operator==(const Itr<TYPE>& rhs) const
{
	if (isItr && rhs.isItr)
		return i == rhs.i;
	if (!isItr && !rhs.isItr)
		return p == rhs.p;
	return false;
}

template<typename TYPE>
inline bool Itr<TYPE>::operator!=(const Itr<TYPE>& rhs) const
{
	return !((*this) == rhs);
}

template<typename TYPE>
inline Itr<TYPE>& Itr<TYPE>::operator++()
{
	if (isItr)
		++i;
	else
		++p;
	return *this;
}

template<typename TYPE>
inline Itr<TYPE> Itr<TYPE>::operator++(int)
{
	ptr<TYPE> rtn(*this);
	++(*this);
	return rtn;
}

template<typename TYPE>
inline Itr<TYPE>& Itr<TYPE>::operator--()
{
	if (isItr) 
		--i;
	else 
		--p;
	return *this;	
}

template<typename TYPE>
inline Itr<TYPE> Itr<TYPE>::operator--(int)
{
	ptr<TYPE> rtn(*this);
	--(*this);
	return rtn;
}

template<typename TYPE>
inline Itr<TYPE>& Itr<TYPE>::operator+=(int64_t offset)
{
	if (isItr)
		i += offset;
	else
		p += offset;
	return (*this);
}

template<typename TYPE>
inline Itr<TYPE> Itr<TYPE>::operator+(int64_t offset) const
{
	ptr<TYPE> rtn(*this);
	rtn += offset;
	return rtn;
}

template<typename TYPE>
inline Itr<TYPE>& Itr<TYPE>::operator-=(int64_t offset)
{
	if (isItr)
		i -= offset;
	else
		p -= offset;
	return (*this);
}

template<typename TYPE>
inline Itr<TYPE> Itr<TYPE>::operator-(int64_t offset)const
{
	ptr<TYPE> rtn(*this);
	rtn -= offset;
	return rtn;
}

template<typename TYPE>
inline int64_t Itr<TYPE>::operator-(const Itr<TYPE>& rhs) const
{
	if (isItr && rhs.isItr)
		return i - rhs.i;
	if (!isItr && !rhs.isItr)
		return p - rhs.p;
			
	return std::numeric_limits<int64_t>::max();
}

template<typename TYPE>
inline TYPE& Itr<TYPE>::operator[](size_t index)
{
	if (isItr)
		return (*i)[index];
	else
		return p[index];
}



