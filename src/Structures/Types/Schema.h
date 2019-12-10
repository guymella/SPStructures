//------------------------------------------------------------------------------
//  VarInt.h
//------------------------------------------------------------------------------

#include "TypeDescr.h"
#include "KeyString.h"
#include "Structures/Containers/SparseArray.h"


class Schema {

	Array<KeyString> Labels;
	Array<TypeDescr> Types;
	SparseArray<Array<uint8_t>> defaults;
	//SparseArray<Darivation> Darivations
	//SparseArray<Array<constraint>> Consraints
};