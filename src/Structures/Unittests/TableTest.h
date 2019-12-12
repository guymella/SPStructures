//------------------------------------------------------------------------------
//  TableTest.h
//  Test Table class.
//------------------------------------------------------------------------------

#include "Structures/Containers/TableStruct.h"
#include "Structures/Containers/Table.h"
//#include "Structures/Containers/Block.h"


bool TestTable()
{

	DBlock b(128);


	TableStruct<2> t(&b);

	size_t zz = t.end() - t.begin();


	size_t us = t.UsedSpace();
	size_t fs = t.FreeSpace();

	size_t s = t.Size();
	//baseTypes tt = b.Type();

	baseTypes bb = *(baseTypes*)t.FindPartitionMem(0);
	bb = *(baseTypes*)t.FindPartitionMem(1);

	auto pp0 = t.GetPartition(0);
	auto pp1 = t.GetPartition(1);
	size_t pps0 = pp0.Size();
	size_t pps1 = pp1.Size();
	pp1.Grow(8);
	pps0 = pp0.Size();
	pps1 = pp1.Size();
	int* p = (int*)pp1.begin();
	p[0] = 5;
	p[1] = 7;

	pp0.Grow(8);
	pps0 = pp0.Size();
	pps1 = pp1.Size();
	p = (int*)pp0.memStart();
	p[0] = 3;
	p[1] = 4;

	p = (int*)pp1.memStart();
	int xx = p[0];
	int yy = p[1];

	p = (int*)pp0.memStart();
	xx = p[0];
	yy = p[1];




	//Test EditableTable
	Table tt(5);

	size_t z = tt.end() - tt.begin();

	size_t uus = tt.UsedSpace();
	size_t ffs = tt.FreeSpace();

	size_t ss = tt.Size();
	//baseTypes tt = b.Type();

	auto p0 = tt.GetPartition(0);
	auto p1 = tt.GetPartition(1);
	size_t ps0 = p0.Size();
	size_t ps1 = p1.Size();
	p1.Grow(8);
	ps0 = p0.Size();
	ps1 = p1.Size();
	int* pa = (int*)p1.begin();
	pa[0] = 5;
	pa[1] = 7;

	p0.Grow(8);
	ps0 = p0.Size();
	ps1 = p1.Size();
	pa = (int*)p0.begin();
	pa[0] = 3;
	pa[1] = 4;

	pa = (int*)p1.begin();
	int x = pa[0];
	int y = pa[1];

	pa = (int*)p0.begin();
	x = pa[0];
	y = pa[1];

	return true;
}