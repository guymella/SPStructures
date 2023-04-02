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
class PackTrieNode : public iTrieNode<TYPE> {
public:




private:
	Array<KeyString> keys;
	Array<TYPE> Values;
	Array<size_t> refs;
	Array<PackTrieNode>* nodes;


};

class PackTrie {
public:
	

private:
	

	

};

