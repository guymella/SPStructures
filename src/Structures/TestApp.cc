//------------------------------------------------------------------------------
//  TestApp.cc
//------------------------------------------------------------------------------

#include "Unittests/ArrayTest.h"
#include "Unittests/BlockTest.h"
#include "Unittests/TableTest.h"
#include "Unittests/SparseArrayTest.h"



int main(void) {

	TestBlock();
	TestTable();
	testArray();
	testSparseArray();

	return 0;
}