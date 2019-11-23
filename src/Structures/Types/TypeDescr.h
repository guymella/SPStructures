//------------------------------------------------------------------------------
//  VarInt.h
//------------------------------------------------------------------------------

#include "Structures/Types.h"

struct BlockDescr {
	uint8_t mem[];
};

struct TableDescr {
	baseTypes type = baseTypes::MemTable;
};

struct TypeDescr {
	baseTypes type = baseTypes::Void;
	bool nullable;
	bool multi;
};