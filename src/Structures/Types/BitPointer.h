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
private:
	TYPE Value() const;
};

template<typename TYPE, uint8_t SIZE>
inline BitRef<TYPE, SIZE>::operator TYPE()
{
	return Value();
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
		adr[0] &= (0xff << (8 - Offset)) & (0xff >> (Offset + bitsleft));
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
