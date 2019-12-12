//------------------------------------------------------------------------------
//  TableTest.h
//  Test Table class.
//------------------------------------------------------------------------------

#include "Structures/Types/Entity.h"
#include <assert.h>
#define CHECK assert


bool TestStruct()
{
	TypeDescr td;
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
	s.AddAttribute(KeyString("dingles"), { baseTypes::string, TypeDescr::setFLags(0,1,0,0,0,0,0)});
	s.AddAttribute(KeyString("dinglet"), { baseTypes::string, TypeDescr::setFLags(0,0,0,0,1,0,0) });

	CHECK(!s.Normalized());
	s.Normalize();
	CHECK(s.Normalized());

	Entity s1(&s);

	return true;
}