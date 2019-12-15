#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iIndexable
    @ingroup Interfaces
    @brief interface for structures that can be accessed by index
*/

template <typename TYPE>
class iIndexable {
public:
	virtual TYPE& operator[](size_t index) = 0;
	virtual const TYPE& operator[](size_t index) const = 0;
};

template <typename TYPE>
class iEditable : public iIndexable<TYPE> {
public:
	//Swap elements an indexes
	virtual void Swap(const size_t& indexA, const size_t& indexB);
};

template <typename TYPE>
class iREditable : public iEditable<TYPE> {
public:

	/// Shift num elements by shift ammount
	virtual void MoveRange(size_t startIndex, size_t numElements, const int64_t& shiftAmmount);
	virtual void MoveRangeLeft(size_t startIndex, size_t numElements, const size_t& shiftAmmount) = 0;
	virtual void MoveRangeRight(size_t startIndex, size_t numElements, const size_t& shiftAmmount) = 0;
	/// Shuffle num elements fill gap by moving overritten ellements in.
	virtual void ShuffleRange(size_t startIndex, size_t numElements, const int64_t& shiftAmmount);
	virtual void ShuffleRangeLeft(size_t startIndex, size_t numElements, const size_t& shiftAmmount) = 0;
	virtual void ShuffleRangeRight(size_t startIndex, size_t numElements, const size_t& shiftAmmount) = 0;
	/// Shuffle num elements fill gap by moving overritten ellements in.
	virtual void Shift(const int64_t& shiftAmmount);
	virtual void ShiftLeft(const size_t& shiftAmmount) = 0;
	virtual void ShiftRight(const size_t& shiftAmmount) = 0;
	//CLear The Array
	virtual void Clear() = 0;
};


template <typename TYPE>
class iDEditable {
public:
	/// copy-insert element at index, keep array order
	virtual void Insert(size_t index, const TYPE& elm) = 0;
	/// move-insert element at index, keep array order
	virtual void Insert(size_t index, TYPE&& elm) = 0;
	/// insert default initialized elements into array
	virtual void insertBlank(const size_t& index, size_t count = 1) = 0;
	/// erase element at index, keep element order, return erased Element
	virtual TYPE Erase(size_t index) = 0;
	/// erase element at index, swap-in front or back element (destroys element ordering), return erased Element
	virtual TYPE EraseSwap(size_t index) = 0;
	/// erase element at index, always swap-in from back (destroys element ordering), return erased Element
	virtual TYPE EraseSwapBack(size_t index) = 0;
	/// erase element at index, always swap-in from front (destroys element ordering), return erased Element
	virtual TYPE EraseSwapFront(size_t index) = 0;
	/// erase a range of elements, keep element order
	virtual void EraseRange(size_t index, size_t num = std::numeric_limits<size_t>::max()) = 0;	
};

template <typename TYPE>
class iSearchable {
public:
	/// find element index with slow linear search, return InvalidIndex if not found
	virtual size_t FindFirstIndexOf(const TYPE& elm, size_t startIndex=0, size_t endIndex = std::numeric_limits<size_t>::max()) const =0;
};

template <typename TYPE>
class iSparse {
public:
	/// find element index with slow linear search, return InvalidIndex if not found
	virtual const TYPE* Exists(size_t index) const=0;
	virtual TYPE* Exists(size_t index)=0;
protected:
	virtual TYPE& GetOrCreate(size_t index)=0;

};

template<typename TYPE>
inline void iEditable<TYPE>::Swap(const size_t& indexA, const size_t& indexB)
{
	TYPE tmp = (*this)[indexA];
	(*this)[indexA] = (*this)[indexB];
	(*this)[indexB] = tmp;
}

template<typename TYPE>
inline void iREditable<TYPE>::MoveRange(size_t startIndex, size_t numElements, const int64_t& shiftAmmount)
{
	if (shiftAmmount > 0)
		MoveRangeRight(startIndex, numElements, shiftAmmount);
	else
		MoveRangeLeft(startIndex, numElements, -shiftAmmount);
}

template<typename TYPE>
inline void iREditable<TYPE>::ShuffleRange(size_t startIndex, size_t numElements, const int64_t& shiftAmmount)
{
	if (shiftAmmount > 0)
		ShuffleRangeRight(startIndex, numElements, shiftAmmount);
	else
		ShuffleRangeLeft(startIndex, numElements, -shiftAmmount);
}

template<typename TYPE>
inline void iREditable<TYPE>::Shift(const int64_t& shiftAmmount)
{
	if (shiftAmmount > 0)
		ShiftRight(shiftAmmount);
	else
		ShiftLeft(-shiftAmmount);
}


