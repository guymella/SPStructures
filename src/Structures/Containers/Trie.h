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
	TrieNode() { nodes = NULL; };
	TrieNode(iDArray<TrieNode<TYPE>>* nodeArray) : nodes(nodeArray) {};
	TrieNode(const TrieNode<TYPE>& rhs);
	TrieNode(TrieNode<TYPE>&& rhs);
	void operator=(const TrieNode<TYPE>& rhs);
	void operator=(TrieNode<TYPE>&& rhs);

	//virtual TYPE& operator[](iKeyString& key) override { return GetOrCreate(key); };
	bool operator==(const TrieNode<TYPE>& rhs) const;
protected:
	virtual TYPE& PushDownPostfix(size_t keyIndex, size_t prefixLen, iKeyString& newKey) override;
	virtual iTrieNode<TYPE>& GetChildNode(size_t index) override;
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
	iDArray<TrieNode<TYPE>>* nodes = NULL;
};

template<typename TYPE>
inline TrieNode<TYPE>::TrieNode(const TrieNode<TYPE>& rhs)
{
	keys = rhs.keys;
	values = rhs.values;
	nodeRefs = rhs.nodeRefs;
	nodes = rhs.nodes;
}

template<typename TYPE>
inline TrieNode<TYPE>::TrieNode(TrieNode<TYPE>&& rhs)
{
	keys = std::move(rhs.keys);
	values = std::move(rhs.values);
	nodeRefs = std::move(rhs.nodeRefs);
	nodes = rhs.nodes;
}

template<typename TYPE>
inline void TrieNode<TYPE>::operator=(const TrieNode<TYPE>& rhs)
{
	keys = rhs.keys;
	values = rhs.values;
	nodeRefs = rhs.nodeRefs;
	nodes = rhs.nodes;
}

template<typename TYPE>
inline void TrieNode<TYPE>::operator=(TrieNode<TYPE>&& rhs)
{
	keys = std::move(rhs.keys);
	values = std::move(rhs.values);
	nodeRefs = std::move(rhs.nodeRefs);
	nodes = rhs.nodes;
}

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
	TYPE* val = values.Exists(keyIndex);
	if (val)
		node.values.PushBack(*val);
	size_t* ref = nodeRefs.Exists(keyIndex);
	if (ref)
		node.nodeRefs.PushBack(*ref);

	Keys()[keyIndex].Truncate(prefixLen);
	nodeRefs[keyIndex] = nodeID;

	if (newKey.Size())
		return node.GetOrCreate(newKey);
	return values[keyIndex];
}

template<typename TYPE>
inline iTrieNode<TYPE>& TrieNode<TYPE>::GetChildNode(size_t index)
{
	size_t* ref = nodeRefs.Exists(index);
	if (ref)
		return (*nodes)[nodeRefs[index]];

	nodeRefs.Insert(index, nodes->Size());
	return nodes->PushBack(TrieNode<TYPE>(nodes));
}
