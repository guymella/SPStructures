//------------------------------------------------------------------------------
//  BitPointer.h
//------------------------------------------------------------------------------

#include "Interfaces/iIterator.h"

template <typename TYPE ,uint8_t SIZE>
struct BitRef {
	uint8_t* Address;
	uint8_t Offset;

	void operator=(TYPE rhs);
	bool operator==(const TYPE& rhs) const;
	bool operator!=(const TYPE& rhs) const;
	TYPE operator++();
	TYPE operator++(int);
	TYPE operator--();
	TYPE operator--(int);
	TYPE operator+=(int64_t increment);
	TYPE operator+(int64_t increment) const;
	TYPE operator-=(int64_t increment);
	TYPE operator-(int64_t increment) const;
	operator TYPE ();
	static BitRef<TYPE,SIZE> Increment(BitRef<TYPE, SIZE> lhs, const size_t& offset);
	static BitRef<TYPE, SIZE> Decrement(BitRef<TYPE, SIZE> lhs, const size_t& offset);
	static BitRef<TYPE, SIZE> Move(const BitRef<TYPE, SIZE>& lhs, const int64_t& offset);
private:
	TYPE Value() const;
};

template <typename TYPE, uint8_t SIZE>
class BitItr : itrBwd<BitRef<TYPE, SIZE>> {
public:
	BitItr(uint8_t* address = NULL, uint8_t offset = 0);
	virtual BitRef<TYPE, SIZE>* Ptr() override { return &ref; };
	virtual const BitRef<TYPE, SIZE>* Ptr() const override { return &ref; };
	virtual BitRef<TYPE, SIZE>& operator*() override { return ref; };
	virtual BitRef<TYPE, SIZE>& operator->() override { return ref; };
	virtual const BitRef<TYPE, SIZE>& operator*() const override { return ref; };
	virtual const BitRef<TYPE, SIZE>& operator->() const override { return ref; };
	virtual bool operator+=(int64_t offset) override;
	virtual bool operator-=(int64_t offset) override;
	virtual BitRef<TYPE, SIZE> operator[](size_t index);
	virtual const BitRef<TYPE, SIZE> operator[](size_t index) const;
	virtual itrBwd<BitRef<TYPE, SIZE>>* MakeCopyBwd() const override;
private:
	BitRef<TYPE, SIZE> ref;
};


template<typename TYPE, uint8_t SIZE>
inline BitRef<TYPE, SIZE>::operator TYPE()
{
	return Value();
}

template<typename TYPE, uint8_t SIZE>
inline BitRef<TYPE, SIZE> BitRef<TYPE, SIZE>::Increment(BitRef<TYPE, SIZE> ref, const size_t& offset)
{
	size_t bits = (offset * SIZE) + ref.Offset;
	ref.Address += bits / 8;
	ref.Offset = bits % 8;
	return ref;
}

template<typename TYPE, uint8_t SIZE>
inline BitRef<TYPE, SIZE> BitRef<TYPE, SIZE>::Decrement(BitRef<TYPE, SIZE> ref, const size_t& offset)
{
	size_t bits = (SIZE * offset);
	ref.Address -= bits / 8;
	if ((bits % 8) > ref.Offset) {
		ref.Address--;
		bits -= ref.Offset;
		ref.Offset = 8;
	}
	ref.Offset -= (uint8_t)bits;

	return ref;
}

template<typename TYPE, uint8_t SIZE>
inline BitRef<TYPE, SIZE> BitRef<TYPE, SIZE>::Move(const BitRef<TYPE, SIZE>& lhs, const int64_t& offset)
{
	return (offset > 0) ? Increment(lhs, offset) : Decrement(lhs, -offset);
}



template<typename TYPE, uint8_t SIZE>
inline TYPE BitRef<TYPE, SIZE>::Value() const
{
	uint8_t bitsleft = SIZE;

	TYPE rtn = 0;
	uint8_t* adr = Address;
	//uint8_t off = Offset;

	//uint8_t tmp = 0xff;
	rtn += (0xff >> Offset)& adr[0];

	if (bitsleft > 8 - Offset)
	{
		bitsleft -= (8 - Offset);
		adr++;
		while (bitsleft / 8) {
			rtn <<= 8;
			rtn += adr[0];
			adr++;
			bitsleft -= 8;
		}
		// last bits
		rtn <<= bitsleft;
		rtn += ((0xff << (8 - bitsleft)) & adr[0]) >> (8 - bitsleft);
	}
	else {
		rtn >>= (8 - Offset - bitsleft);
	}
	return rtn;
}

template<typename TYPE, uint8_t SIZE>
inline void BitRef<TYPE, SIZE>::operator=(TYPE rhs)
{
	uint8_t bitsleft  = SIZE;

	uint8_t* adr = Address;

	if (bitsleft>8-Offset) {
		bitsleft -= (8 - Offset);
		adr++;
	
		adr += bitsleft / 8;
		bitsleft = bitsleft % 8;	
		//uint8_t off = Offset;
		//uint8_t tmp = rhs % 0xff;

		adr[0] &= (0xff >> bitsleft);
		adr[0] |= (rhs % 0xff << (8 - bitsleft));
		rhs >>= bitsleft;

		while (SIZE - bitsleft >= 8) {
			adr--;
			adr[0] = rhs % 0xff;
			rhs >>= 8;
			bitsleft += 8;
		}

		if (SIZE - bitsleft > 0) {
			adr--;
			adr[0] &= 0xff << (SIZE - bitsleft);
			adr[0] |= (0xff >> (Offset)) & (rhs % 0xff);
		}	
	}
	else {
		uint8_t tmp = (rhs % 0xff) & (0xff >> (8 - bitsleft));
		adr[0] &= ((0xff << (8 - Offset)) | (0xff >> (Offset + bitsleft)));
		adr[0] |= tmp << (8 - Offset - bitsleft);
	}
}

template<typename TYPE, uint8_t SIZE>
inline bool BitRef<TYPE, SIZE>::operator==(const TYPE& rhs) const
{
	return (TYPE)(*this) == rhs;
}

template<typename TYPE, uint8_t SIZE>
inline bool BitRef<TYPE, SIZE>::operator!=(const TYPE& rhs) const
{
	return !((*this) == rhs);
}

template<typename TYPE, uint8_t SIZE>
inline TYPE BitRef<TYPE, SIZE>::operator++()
{
	return (*this) += 1;
}

template<typename TYPE, uint8_t SIZE>
inline TYPE BitRef<TYPE, SIZE>::operator++(int)
{
	TYPE rtn = (*this);
	(*this)++;
	return rtn;
}

template<typename TYPE, uint8_t SIZE>
inline TYPE BitRef<TYPE, SIZE>::operator--()
{
	return (*this) -= 1;
}

template<typename TYPE, uint8_t SIZE>
inline TYPE BitRef<TYPE, SIZE>::operator--(int)
{
	TYPE rtn = (*this);
	(*this)--;
	return rtn;
}

template<typename TYPE, uint8_t SIZE>
inline TYPE BitRef<TYPE, SIZE>::operator+=(int64_t increment)
{
	(*this) = (*this) + increment;
	return (*this);
}

template<typename TYPE, uint8_t SIZE>
inline TYPE BitRef<TYPE, SIZE>::operator+(int64_t increment) const
{
	return Value() + (TYPE)increment;
}

template<typename TYPE, uint8_t SIZE>
inline TYPE BitRef<TYPE, SIZE>::operator-=(int64_t increment)
{
	(*this) = (*this) - increment;
	return (*this);
}

template<typename TYPE, uint8_t SIZE>
inline TYPE BitRef<TYPE, SIZE>::operator-(int64_t increment) const
{
	return Value() - (TYPE)increment;
}

template<typename TYPE, uint8_t SIZE>
inline BitItr<TYPE, SIZE>::BitItr(uint8_t* address, uint8_t offset)
{
	ref.Address = address;
	ref.Offset = offset;
}

template<typename TYPE, uint8_t SIZE>
inline bool BitItr<TYPE, SIZE>::operator+=(int64_t offset)
{
	ref = BitRef<TYPE, SIZE>::Move(ref, offset);
	return true;
}

template<typename TYPE, uint8_t SIZE>
inline bool BitItr<TYPE, SIZE>::operator-=(int64_t offset)
{
	ref = BitRef<TYPE, SIZE>::Move(ref, -offset);
	return true;
}

template<typename TYPE, uint8_t SIZE>
inline BitRef<TYPE, SIZE> BitItr<TYPE, SIZE>::operator[](size_t index)
{
	return BitRef<TYPE, SIZE>::Move(ref, index);
}

template<typename TYPE, uint8_t SIZE>
inline const BitRef<TYPE, SIZE> BitItr<TYPE, SIZE>::operator[](size_t index) const
{
	return BitRef<TYPE, SIZE>::Move(ref, index);
}

template<typename TYPE, uint8_t SIZE>
inline itrBwd<BitRef<TYPE, SIZE>>* BitItr<TYPE, SIZE>::MakeCopyBwd() const
{
	return new BitItr<TYPE, SIZE>(ref.Address, ref.Offset);
}
