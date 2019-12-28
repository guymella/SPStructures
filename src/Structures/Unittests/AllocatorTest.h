//------------------------------------------------------------------------------
//  AllocatorTest.h
//  Test Block class.
//------------------------------------------------------------------------------


#include "structures/Containers/Allocator.h"
//#include "BlockTest.h"

bool AllocatorTest()
{
	AllocatorTable a;

	a.PushBack(1024);

	CHECK(iBlockTest_Iterate(a.begin()[0]));

	BlockDV b(128);
	a.PushBack();
	a.begin()[0] = b;

	CHECK(!BlockTest_IterateValidate(a.begin()[0], 0, 128));
	CHECK(iBlockTest_Iterate(b));
	a.begin()[0] = b;
	CHECK(BlockTest_IterateValidate(a.begin()[0], 0, 128));

	//CHECK(BlockDTest<AllocatorBlock>());
	AllocatorTable aa;
	aa.PushBack();
	aa.PushBack();
	aa.PushBack();
	aa.PushBack();
	aa.PushBack();
	aa.PushBack();
	aa.PushBack();
	aa.PushBack();
	aa.PushBack();
	aa.PushBack();
	return true;
}