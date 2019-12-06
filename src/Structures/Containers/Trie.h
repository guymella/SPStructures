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
#include "Structures/Types/KeyString.h"




template <typename TYPE>
class TrieNode : public iTrieNode<TYPE> {
public:
	TrieNode() {};
protected:
	virtual TYPE* ValueExists(size_t index) override;
	virtual TYPE& PushDownPostfix(size_t prefixLen, iKeyString newKey) override;
	virtual iTrieNode<TYPE>& GetChildNode(size_t index) override;
	virtual iTrieNode<TYPE>& InsertChildNode(size_t index) override;
	virtual TYPE& GetValue(size_t index) override;
	virtual iDArray<KeyString>& Keys() override;
	virtual iDArray<TYPE>& Values() override;



private:
	Array<KeyString> keys;
	Array<TYPE> Values;
	Array<PackTrieNode> nodes;
};
