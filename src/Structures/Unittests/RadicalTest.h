//------------------------------------------------------------------------------
//  AllocatorTest.h
//  Test Block class.
//------------------------------------------------------------------------------


#include "structures/Containers/Radical.h"
//#include "BlockTest.h"

bool RadicalTest()
{
	CHECK(true);
	AllocatorTable a;
	RadAdapter r(&a);

	keyBuilder k1("helloWorld");
	CHECK(!r.Exists(k1));

	iBlockD& v1 = r.GetOrCreate(k1);
	CHECK(v1.Empty());
	v1.ReserveBack(21);
	CHECK(r.GetOrCreate(k1).Size() == 21);

	keyBuilder k2("sup");
	iBlockD& v2 = r.GetOrCreate(k2);
	v2.ReserveBack(22);
	CHECK(r.GetOrCreate(k2).Size() == 22);

	keyBuilder k3("adaboy");
	iBlockD& v3 = r.GetOrCreate(k3);
	v3.ReserveBack(23);
	CHECK(r.GetOrCreate(k3).Size() == 23);

	CHECK(r.GetOrCreate(k1).Size() == 21);
	CHECK(r.GetOrCreate(k2).Size() == 22);
	CHECK(r.GetOrCreate(k3).Size() == 23);

	//split key
	keyBuilder k4("hello");
	iBlockD& v4 = r.GetOrCreate(k4);
	v4.ReserveBack(24);
	CHECK(r.GetOrCreate(k4).Size() == 24);

	//extend key
	keyBuilder k5("helloWorlds");
	iBlockD& v5 = r.GetOrCreate(k5);
	v5.ReserveBack(25);
	CHECK(r.GetOrCreate(k5).Size() == 25);

	//add split key
	keyBuilder k6("helloFriend");
	iBlockD& v6 = r.GetOrCreate(k6);
	v6.ReserveBack(26);
	CHECK(r.GetOrCreate(k6).Size() == 26);

	//split again
	keyBuilder k7("hell");
	iBlockD& v7 = r.GetOrCreate(k7);
	v7.ReserveBack(27);
	CHECK(r.GetOrCreate(k7).Size() == 27);

	return true;
}