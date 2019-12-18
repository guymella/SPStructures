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
	td.type = baseTypes::int32;

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

	s.AddAttribute("dingleCount", baseTypes::uint32);
	s.AddAttribute(KeyString("name"), { baseTypes::string,0 });
	s.AddAttribute(KeyString("dingles"), { baseTypes::string, Types::TypeDescr::setFLags(0,1,0,0,0,0,0)});
	s.AddAttribute(KeyString("dinglet"), { baseTypes::string, Types::TypeDescr::setFLags(0,0,0,0,1,0,0) });

	Entity s1(&s);

	return true;
}