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
	virtual const TYPE* Exists(iKeyString& key) const;
	virtual TYPE& GetOrCreate(iKeyString& key);
	virtual TYPE& operator[](iKeyString& key) override {return GetOrCreate(key);};
protected:
	virtual TYPE* ValueExists(size_t index) = 0;
	virtual TYPE& PushDownPostfix(size_t prefixLen, iKeyString newKey) = 0;
	virtual iTrieNode<TYPE>& GetChildNode(size_t index) = 0;
	virtual iTrieNode<TYPE>& InsertChildNode(size_t index) = 0;
	virtual TYPE& GetValue(size_t index) = 0;
	virtual iDArray<KeyString>& Keys() = 0;
	virtual iDArray<TYPE>& Values() = 0;	
};

template<typename TYPE>
inline const TYPE* iTrieNode<TYPE>::Exists(iKeyString& checkKey) const
{
	iDArray<KeyString>& k = Keys();
	for (size_t i = 0; i < k.Size(); i++) {
		keyCompare c = k[i].CompareTo(checkKey);
		if (c.commonPrefix) { //at least partial match
			if (!c.Postfix0) { //no remainder
				if (c.Postfix1) {//checkKey has remainder
					checkKey.Tare(c.commonPrefix);
					return GetChildNode(i).Exists(checkKey);
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
			checkKey.Tare(c.commonPrefix);
			if (!c.Postfix0) { //no remainder key full match
				if (c.Postfix1) //checkKey has remainder					
					return GetChildNode(i).GetOrCreate(checkKey);
				else //FULL MATCH
					return GetValue(i);
			}
			else {//key remainder = partial match. split key and push into child node
				return PushDownPostfix(c.commonPrefix, checkKey);
			}
		}//no match

		if (!c.LessThan) {//past point key should be
			//insert key and value before i.
			Keys().Insert(i, checkKey);
			Values().Insert(i, TYPE());
			InsertChildNode(i);
			return Values()[i];
		}
	}
	return NULL;
}
