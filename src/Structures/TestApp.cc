//------------------------------------------------------------------------------
//  TestApp.cc
//------------------------------------------------------------------------------

#include "Unittests/ArrayTest.h"
#include "Unittests/BlockTest.h"
#include "Unittests/TableTest.h"
#include "Unittests/SparseArrayTest.h"
#include "Unittests/TrieTest.h"



int main(void) {

	TestBlock();
	TestTable();
	testArray();
	testSparseArray();
	TestTrie();

	return 0;
}