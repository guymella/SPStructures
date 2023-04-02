//------------------------------------------------------------------------------
//  BlockTest.h
//  Test Block class.
//------------------------------------------------------------------------------


#include "structures/Containers/Block.h"
#include "structures/Containers/BlockP.h"
#include "structures/Containers/BlockPP.h"

#include <assert.h>
#define CHECK assert

bool iBlockTest_Empty(iBlock* a)
{
	if (a->Empty()) {
		CHECK(!a->Size());
		//CHECK(!(a.end() - a.begin()));
		return true;
	}
	else {
		CHECK(a->Size());
		//CHECK(a.Size() == (a.end() - a.begin()));
	}

	return false;
}

bool iBlockTest_Iterate(iBlock* a)
{
	if (a->Empty()) 
		return false;
	else {
		size_t size = a->Size();
		CHECK(size);
		//CHECK(size == (a->end() - a->begin()));
		for (size_t i = 0; i < size; i++)
			a->begin(i)[0] = (i % 255);
		for (size_t i = 0; i < size; i++)
			CHECK(a->begin()[i] == (i % 255));
		auto itr = a->begin();
		size_t i = 0;
		while (itr != a->end())
			CHECK(*(itr++) == (i++ % 255));

		itr = a->end();
		i = size;
		while (itr != a->begin())
			CHECK(*(--itr) == (--i % 255));

		for (size_t i = 0; i < size; i++) {
			size_t ind = rand() % size;
			CHECK(*a->begin(ind) == ind % 255);
			CHECK(a->begin()[ind] == ind % 255);
			CHECK(*a->end(-(int64_t)(size - ind)) == ind % 255);
			CHECK(((Itr<uint8_t>)a->end())[-(int64_t)(size - ind)] == ind % 255);
		}
	}

	return true;
}

bool iBlockTest_IterateValidate(iBlock& a, size_t startIndex = 0, size_t endIndex = std::numeric_limits<size_t>::max(), int64_t offset =0)
{
	if (a.Empty())
		return false;
	else {
		size_t size = a.Size();
		CHECK(size);
		CHECK(size == (a.end() - a.begin()));
		if (endIndex > size)
			endIndex = size;

		size = endIndex - startIndex;

		for (size_t i = startIndex; i < endIndex; i++)
			if (!(a.begin()[i] == ((i + offset) % 255))) 
				return false;
		
		auto itr = a.begin(startIndex);
		size_t i = 0;
		while (itr != a.begin(endIndex))
			if (!*(itr++) == ((i++ + offset) % 255)) 
				return false;

		itr = a.begin(endIndex);
		i = size;
		while (itr != a.begin(startIndex))
			if (!*(--itr) == ((--i + offset) % 255)) 
				return false;

		for (size_t i = 0; i < size; i++) {
			size_t ind = rand() % size;
			if (!*a.begin(ind) == (ind + offset) % 255) 
				return false;
			if (!a.begin()[ind] == (ind + offset) % 255) 
				return false;
			//CHECK(*a.end(-(int64_t)(size - ind)) == (ind + offset) % 255);
			//CHECK(((Itr<uint8_t>)a.end())[-(int64_t)(size - ind)] == (ind + offset) % 255);
		}
	}

	return true;
}



template<typename BLOCK>
bool BlockTest_Empty(BLOCK& a) { return iBlockTest_Empty((iBlock *) & a); }

template<typename BLOCK>
bool BlockTest_Iterate(BLOCK& a) {	return iBlockTest_Iterate((iBlock*)&a); }

template<typename BLOCK>
bool BlockTest_IterateValidate(BLOCK& a, size_t startIndex = 0, size_t endIndex = std::numeric_limits<size_t>::max(), int64_t offset = 0) { 
	return iBlockTest_IterateValidate(a,startIndex,endIndex,offset); 
}

template <size_t SIZE = 1024>
bool BlockSTest()
{
	//connot construct empty blockS
	
	Block<SIZE> b;
	CHECK(!BlockTest_Empty(b));

	CHECK(BlockTest_Iterate(b));

	return true;
}

//template<typename BLOCK>
//bool BlockDTest()
//{
//	BLOCK a;
//	CHECK(BlockTest_Empty(a));
//	CHECK(!BlockTest_Iterate(a));
//
//	BLOCK b(1024);
//	CHECK(!BlockTest_Empty(b));
//	CHECK(BlockTest_Iterate(b));
//
//	//test grow
//
//	a.ReserveBack(512);
//	CHECK(!BlockTest_Empty(a));
//	CHECK(a.Size() == 512);
//	CHECK(a.Capacity() == 512);
//	CHECK(BlockTest_Iterate(a));
//	CHECK(BlockTest_IterateValidate(a));
//
//	a.Resize(1024);
//	CHECK(!BlockTest_Empty(a));
//	CHECK(a.Size() == 1024);
//	CHECK(a.Capacity() == 1024);
//	CHECK(BlockTest_IterateValidate(a,0,512));
//	CHECK(!BlockTest_IterateValidate(a, 0, 1024));
//	CHECK(BlockTest_Iterate(a));
//	CHECK(BlockTest_IterateValidate(a));
//
//	a.ReserveFront(1024);
//	CHECK(!BlockTest_Empty(a));
//	CHECK(a.Size() == 2048);
//	CHECK(a.Capacity() == 2048);
//	CHECK(BlockTest_IterateValidate(a, 1024, 1024));
//	CHECK(!BlockTest_IterateValidate(a, 0, 1024));
//	CHECK(BlockTest_Iterate(a));
//	CHECK(BlockTest_IterateValidate(a));
//
//	BLOCK c(a);
//	b = std::move(a);
//
//	CHECK(BlockTest_Empty(a));
//
//	int64_t comp = Itr<uint8_t>::Compare(c.begin(), b.begin(), b.end());
//
//	CHECK(comp == c.Size());
//
//	return true;
//}

//void dbg() {
//
//	BlockDV b(1024);
//	//CHECK(!BlockTest_Empty(b));
//	//CHECK(BlockTest_Iterate(b));
//}



bool BlockTest() {
	Block a;
	Block b(1024);

	CHECK(BlockTest_Empty(a));
	CHECK(BlockTest_Iterate(b));
	CHECK(BlockTest_IterateValidate(b));

	BlockP p;
	BlockP pp(8);

	CHECK(p.Size() == 0);
	CHECK(pp.Size() == 256);

	CHECK(BlockTest_Empty(p));
	CHECK(BlockTest_Iterate(pp));
	CHECK(BlockTest_IterateValidate(pp));


	CHECK(sizeof(Pptr) == 8);

	BlockPP P;
	BlockPP PP(8);

	CHECK(P.Size() == 0);
	CHECK(PP.Size() == 256);

	CHECK(BlockTest_Empty(P));
	CHECK(!BlockTest_Empty(PP));
	CHECK(BlockTest_Iterate(PP));
	CHECK(BlockTest_IterateValidate(PP));


	//test Move
	BlockPP m = std::move(PP);
	CHECK(BlockTest_Empty(PP));
	CHECK(BlockTest_IterateValidate(m));

	BlockP M = std::move(m);
	CHECK(BlockTest_Empty(m));
	CHECK(BlockTest_IterateValidate(M));

	BlockP MM = std::move(M);
	CHECK(BlockTest_Empty(M));
	CHECK(BlockTest_IterateValidate(MM));

	return true;
}