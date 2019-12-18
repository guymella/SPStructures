//------------------------------------------------------------------------------
//  TestApp.cc
//------------------------------------------------------------------------------


#include "Unittests/BlockTest.h"
#include "Unittests/TableTest.h"
#include "Unittests/ArrayTest.h"
#include "Unittests/SparseArrayTest.h"
#include "Unittests/TrieTest.h"
#include "Unittests/StructTest.h"



int main(void) {

	TestBlock();
	TestTable();
	testArray();
	testSparseArray();
	TestTrie();
	TestStruct();

	return 0;
}