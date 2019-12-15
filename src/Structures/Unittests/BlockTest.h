//------------------------------------------------------------------------------
//  BlockTest.h
//  Test Block class.
//------------------------------------------------------------------------------


#include "structures/Containers/Block.h"

bool TestBlock()
{
	DBlock a(8);
	int* p = (int*) a.begin().Ptr();
	p[0] = 5;
	p[1] = 7;

	a.Grow(16);

	p = (int*)a.begin().Ptr();
	int x = p[0];
	int y = p[1];
	p[2] = 5;
	p[3] = 7;

	return true;
}