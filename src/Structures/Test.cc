//------------------------------------------------------------------------------
//  Block.cc
//------------------------------------------------------------------------------
#include "structures/Containers/Block.h"

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

	return 0;
}