//------------------------------------------------------------------------------
//  BlockTest.h
//  Test Block class.
//------------------------------------------------------------------------------


#include "structures/Containers/Trie.h"
#include <assert.h>
#define CHECK assert

bool TestTrie()
{
	Array<TrieNode<int>> Trie;	;
	Trie.PushBack(TrieNode<int>(&Trie));

	KeyString k1("helloworld");
	CHECK(k1.StringData().Size() == 10);
	CHECK(k1.StringData()[0] == 'h');
	CHECK(k1.StringData()[5] == 'w');
	KeyString k2("aman");
	KeyString k3("supWorld");
	KeyString k4("helloworlds");
	KeyString k5("hello");
	KeyString k6("nanunanu");


	auto& nn = Trie.Front();
	
	nn[k1] = 4;
	nn[k2] = 5;
	nn[k3] = 6;

	CHECK(*nn.Exists(k1) == 4);
	CHECK(*nn.Exists(k2) == 5);
	CHECK(*nn.Exists(k3) == 6);

	CHECK(nn[k1] == 4);
	CHECK(nn[k2] == 5);
	CHECK(nn[k3] == 6);
	CHECK(Trie.Size() == 1);
	
	nn[k4] = 7;
	CHECK(*nn.Exists(k4) == 7);
	CHECK(nn[k4] == 7);
	CHECK(Trie.Size() == 2);

	CHECK(*nn.Exists(k1) == 4);
	CHECK(*nn.Exists(k2) == 5);
	CHECK(*nn.Exists(k3) == 6);
	CHECK(nn[k1] == 4);
	CHECK(nn[k2] == 5);
	CHECK(nn[k3] == 6);
	
	nn[k5] = 8;
	CHECK(*nn.Exists(k5) == 8);
	CHECK(nn[k5] == 8);
	CHECK(Trie.Size() == 3);

	CHECK(*nn.Exists(k1) == 4);
	CHECK(*nn.Exists(k2) == 5);
	CHECK(*nn.Exists(k3) == 6);
	CHECK(*nn.Exists(k4) == 7);	
	CHECK(nn[k1] == 4);
	CHECK(nn[k2] == 5);
	CHECK(nn[k3] == 6);
	CHECK(nn[k4] == 7);

	return true;
}