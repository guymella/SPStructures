//------------------------------------------------------------------------------
//  TableTest.h
//  Test Table class.
//------------------------------------------------------------------------------

#include "Structures/Containers/Table.h"
#include "Structures/Containers/Block.h"

bool TestTable()
{

	DBlock b(128);


	Table t(&b, 2);

	size_t zz = ((size_t)t.MemEnd() - (size_t)t.MemStart());

	size_t us = t.UsedSpace();
	size_t fs = t.FreeSpace();

	size_t s = t.Size();
	//baseTypes tt = b.Type();

	baseTypes bb = *(baseTypes*)t.FindPartitionMem(0);
	bb = *(baseTypes*)t.FindPartitionMem(1);

	iDBlock* pp0 = t.GetPartition(0);
	iDBlock* pp1 = t.GetPartition(1);
	size_t pps0 = pp0->Size();
	size_t pps1 = pp1->Size();
	pp1->Grow(8);
	pps0 = pp0->Size();
	pps1 = pp1->Size();
	int* p = (int*)pp1->memStart();
	p[0] = 5;
	p[1] = 7;

	pp0->Grow(8);
	pps0 = pp0->Size();
	pps1 = pp1->Size();
	p = (int*)pp0->memStart();
	p[0] = 3;
	p[1] = 4;

	p = (int*)pp1->memStart();
	int xx = p[0];
	int yy = p[1];

	p = (int*)pp0->memStart();
	xx = p[0];
	yy = p[1];

	return true;
}