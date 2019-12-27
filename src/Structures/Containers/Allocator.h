#pragma once
//------------------------------------------------------------------------------
/*
    @class Structures::Allocator
    @ingroup Block
    @brief memory allocator. allocates storage and handles fragmentation
*/

//#include <stdint.h>
//#include <string.h>
//#include <memory>
//#include "Array.h"
#include "Interfaces\iBlock.h"

class Allocator : iBlock{
public:
	Allocator() {};
	
	// indicates presence of subtablte Table();
	virtual bool ContainsTable() const override { return true; };
	//access subtable
	virtual iTable<>& Table() override = 0;
	//const access subtable
	virtual const iTable<>& Table() const override = 0;
	//Size of table
	virtual size_t Size() const override = 0;
	/// C++ conform begin
	virtual Itr<uint8_t> begin(const int64_t& offset = 0) override = 0;
	/// C++ conform begin
	virtual Itr<const uint8_t> begin(const int64_t& offset = 0) const override = 0;
	/// C++ conform end
	virtual Itr<uint8_t> end(const int64_t& offset = 0) override = 0;
	/// C++ conform end
	virtual Itr<const uint8_t> end(const int64_t& offset = 0) const override = 0;
protected:
	iBlock* blocks();
	
};