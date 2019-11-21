//------------------------------------------------------------------------------
//  Block.cc
//------------------------------------------------------------------------------
#include "structures/Containers/Block.h"
#include "Structures/Containers/Table.h"

int main(void) {
	
	Block a(8);
	int* p = (int*)a.memStart();
	p[0] = 5;
	p[1] = 7;

	



	a.Grow(16);

	p = (int*)a.memStart();
	int x = p[0];
	int y = p[1];
	p[2] = 5;
	p[3] = 7;

	Block b(128);
	

	Table t(&b,2);

	size_t zz = ((size_t)t.MemEnd() - (size_t)t.MemStart());

	size_t us = t.UsedSpace();
	size_t fs = t.FreeSpace();	

	size_t s = t.Size();
	baseTypes tt = b.Type();

	baseTypes bb = *(baseTypes * )t.FindPartitionMem(0);
	bb = *(baseTypes*)t.FindPartitionMem(1);

	iBlock* pp0 = t.GetPartition(0);
	iBlock* pp1 = t.GetPartition(1);
	size_t pps0 = pp0->Size();
	size_t pps1 = pp1->Size();
	pp1->Grow(8);
	pps0 = pp0->Size();
	pps1 = pp1->Size();
	p = (int*)pp1->memStart();
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


	return 0;
}