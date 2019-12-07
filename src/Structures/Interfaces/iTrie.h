#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iTable
    @ingroup Interfaces
    @brief interface for Procedural inline Memory Table. subdivides a block into multiple dynamic partitions.
*/
#include <stdint.h>
#include "iBlock.h"

#include "Structures/Types/KeyString.h"

template <typename TYPE>
class iTrieNode {
public:
	virtual TYPE* Exists(iKeyString& checkKey);
	virtual const TYPE* Exists(iKeyString& key) const;
	virtual TYPE& GetOrCreate(iKeyString& key);
	virtual TYPE& operator[](iKeyString& key) {return GetOrCreate(key);};
protected:
	virtual TYPE* ValueExists(size_t index) { return Values().Exists(index); };
	virtual const TYPE* ValueExists(size_t index) const { return Values().Exists(index); };
	virtual TYPE& PushDownPostfix(size_t keyIndex, size_t prefixLen, iKeyString& newKey) = 0;
	virtual iTrieNode<TYPE>& GetChildNode(size_t index) = 0;
	virtual const iTrieNode<TYPE>& GetChildNode(size_t index) const = 0;
	//virtual iTrieNode<TYPE>& InsertChildNode(size_t index) = 0;
	virtual TYPE& InsertKey(size_t index, iKeyString& newKey);
	virtual TYPE& GetValue(size_t index) { return (Values().Exists(index))[0]; };
	virtual iDArray<KeyString>& Keys() = 0;
	virtual const iDArray<KeyString>& Keys() const = 0;
	virtual iSparseArray<TYPE>& Values() = 0;
	virtual const iSparseArray<TYPE>& Values() const = 0;
};

template<typename TYPE>
inline TYPE* iTrieNode<TYPE>::Exists(iKeyString& checkKey)
{
	
	iDArray<KeyString>& k = Keys();
	for (size_t i = 0; i < k.Size(); i++) {
		keyCompare c = k[i].CompareTo(checkKey);
		if (c.commonPrefix) { //at least partial match
			if (!c.Postfix0) { //no remainder
				if (c.Postfix1) {//checkKey has remainder
					size_t tare = checkKey.Tare();
					checkKey.Tare(c.commonPrefix);
					TYPE* rtn = GetChildNode(i).Exists(checkKey);
					checkKey.Tare() = tare;
					return rtn;
				}
				else
					return ValueExists(i);
			}
			else //key remainder =  no match;
				break;
		}
		
		if (!c.LessThan)//past point key should be
			break;
	}
	return NULL;
}

template<typename TYPE>
inline const TYPE* iTrieNode<TYPE>::Exists(iKeyString& checkKey) const
{
	const iDArray<KeyString>& k = Keys();
	for (size_t i = 0; i < k.Size(); i++) {
		keyCompare c = k[i].CompareTo(checkKey);
		if (c.commonPrefix) { //at least partial match
			if (!c.Postfix0) { //no remainder
				if (c.Postfix1) {//checkKey has remainder
					size_t tare = checkKey.Tare();
					checkKey.Tare(c.commonPrefix);
					const TYPE* rtn = GetChildNode(i).Exists(checkKey);
					checkKey.Tare() = tare;
					return rtn;
				}
				else
					return ValueExists(i);
			}
			else //key remainder =  no match;
				break;
		}

		if (!c.LessThan)//past point key should be
			break;
	}
	return NULL;
}

template<typename TYPE>
inline TYPE& iTrieNode<TYPE>::GetOrCreate(iKeyString& checkKey)
{
	iDArray<KeyString>& k = Keys();
	for (size_t i = 0; i < k.Size(); i++) {
		keyCompare c = k[i].CompareTo(checkKey);
		if (c.commonPrefix) { //at least partial match
			if (!c.Postfix0) { //no remainder key full match
				if (c.Postfix1) { //checkKey has remainder
					size_t tare = checkKey.Tare();
					checkKey.Tare(c.commonPrefix);
					TYPE& rtn = GetChildNode(i).GetOrCreate(checkKey);
					checkKey.Tare() = tare;
					return rtn;
				} else //FULL MATCH
					return GetValue(i);
			}
			else {//key remainder = partial match. split key and push into child node
				size_t tare = checkKey.Tare();
				checkKey.Tare(c.commonPrefix);
				TYPE& rtn = PushDownPostfix(i, c.commonPrefix, checkKey);
				checkKey.Tare() = tare;
				return rtn; 
			}
		}//no match

		if (!c.LessThan) {//past point key should be
			return InsertKey(i, checkKey);			
		}
	}
	return InsertKey(Keys().Size(), checkKey);
}

template<typename TYPE>
inline TYPE& iTrieNode<TYPE>::InsertKey(size_t index, iKeyString& newKey)
{

	Keys().Insert(index, KeyString(newKey));
	Values().Insert(index, TYPE());
	return Values()[index];
}
