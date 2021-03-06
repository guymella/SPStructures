//------------------------------------------------------------------------------
//  TableTest.h
//  Test Table class.
//------------------------------------------------------------------------------
//#include "Structures/Types/schema.h"
#include "Structures/Types/Entity.h"

#include <assert.h>
#define CHECK assert


bool TestStruct()
{
	Types::TypeDescr td;
	td.type = Types::baseTypes::int32;

	CHECK(!td.Nullable());
	CHECK(!td.Multiple());
	CHECK(!td.Sparse());
	//CHECK(!td.CustomDefault());
	CHECK(!td.Constrained());
	CHECK(!td.Derived());
	CHECK(!td.Cached());
	CHECK(!td.Subscribable());

	td.Nullable(true);
	//td.CustomDefault(true);

	CHECK(td.Nullable());
	CHECK(!td.Multiple());
	CHECK(!td.Sparse());
	//CHECK(td.CustomDefault());
	CHECK(!td.Constrained());
	CHECK(!td.Derived());
	CHECK(!td.Cached());
	CHECK(!td.Subscribable());

	td.Nullable(false);
	//td.CustomDefault(true);
	td.Cached(true);

	CHECK(!td.Nullable());
	CHECK(!td.Multiple());
	CHECK(!td.Sparse());
	//CHECK(td.CustomDefault());
	CHECK(!td.Constrained());
	CHECK(!td.Derived());
	CHECK(td.Cached());
	CHECK(!td.Subscribable());



	Schema s;

	s.AddAttribute("dingleCount", Types::baseTypes::uint32);
	s.AddAttribute("dingleratio", Types::baseTypes::float64);
	s.AddAttribute("dingleletter", Types::baseTypes::chr);
	s.AddAttribute(KeyString("name"), { Types::baseTypes::String,0 });
	s.AddAttribute(KeyString("dingles"), { Types::baseTypes::String, Types::TypeDescr::setFLags(0,1,0,0,0,0,0)});
	s.AddAttribute(KeyString("dinglet"), { Types::baseTypes::String, Types::TypeDescr::setFLags(0,0,0,0,1,0,0) });
	s.AddAttribute(KeyString("dingtime"), { Types::baseTypes::float32, Types::TypeDescr::setFLags(1,0,0,0,0,0,0) });
	s.AddAttribute(KeyString("dingwimple"), { Types::baseTypes::uint128, Types::TypeDescr::setFLags(0,0,1,0,0,0,0) });

	size_t size = s.SizeOfFixed();

	CHECK(size == 18);

	uint8_t set[3] = { 0,0,0 };
	BitItr<uint8_t,1> b(set);
	b[0] = true;
	b[1] = false;
	b[2] = false;
	b[3] = true;

	CHECK(*b);
	b += 1;
	CHECK(!*b);
	b += 1;
	CHECK(!*b);
	b += 1;
	CHECK(*b);

	Entity s1(&s);

	

	return true;
}