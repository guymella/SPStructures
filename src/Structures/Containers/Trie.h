#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Block
    @ingroup Block
    @brief inline Block of dynamic Memory, Self Allocating on heap
*/

#include "Structures/Types.h"
#include "Structures/Interfaces/iTrie.h"
#include "Structures/Containers/Partition.h"
//#include "Structures/Types/KeyString.h"


template <typename TYPE>
class TrieNode : public iTrieNode<TYPE> {
public:
	TrieNode() {};
	TrieNode(iDArray<TrieNode<TYPE>>* nodeArray) : nodes(nodeArray) {};
	bool operator==(const TrieNode<TYPE>& rhs) const;
protected:
	virtual TYPE& PushDownPostfix(size_t keyIndex, size_t prefixLen, iKeyString& newKey) override;
	virtual iTrieNode<TYPE>& GetChildNode(size_t index) override { return (*nodes)[nodeRefs[index]]; };
	virtual const iTrieNode<TYPE>& GetChildNode(size_t index) const override { return (*nodes)[nodeRefs[index]]; };
	virtual iDArray<KeyString>& Keys()override { return keys; };
	virtual const iDArray<KeyString>& Keys() const override { return keys; };
	virtual iSparseArray<TYPE>& Values() override { return values; };
	virtual const iSparseArray<TYPE>& Values() const override { return values; };
	//virtual TYPE& InsertKey(size_t index, iKeyString& newKey) override;

private:
	Array<KeyString> keys;
	SparseArray<TYPE> values;
	SparseArray<size_t> nodeRefs;
	iDArray<TrieNode<TYPE>>* nodes;
};

template<typename TYPE>
inline bool TrieNode<TYPE>::operator==(const TrieNode<TYPE>& rhs) const
{
	return (rhs.Keys().Size() == Keys().Size() && rhs.nodes == nodes);
}

template<typename TYPE>
inline TYPE& TrieNode<TYPE>::PushDownPostfix(size_t keyIndex, size_t prefixLen, iKeyString& newKey)
{
	//Make new node
	size_t nodeID = nodes->Size();

	TrieNode<TYPE>& node = nodes->PushBack(TrieNode<TYPE>(nodes));

	KeyString oldKey(Keys()[keyIndex]);
	oldKey.Tare(prefixLen);
	KeyString post(oldKey);
	node.keys.PushBack(oldKey);
	node.values.PushBack(values[keyIndex]);
	node.nodeRefs.PushBack(nodeRefs[keyIndex]);

	Keys()[keyIndex].Truncate(prefixLen);
	nodeRefs[keyIndex] = nodeID;

	return node.GetOrCreate(newKey);
}
