//------------------------------------------------------------------------------
//  VarInt.h
//------------------------------------------------------------------------------

#include "Structures/Interfaces/iProcArray.h"

class VarUInt {
public:
	VarUInt(uint8_t* source) : src(source) {};
	long long Size();

private:
	uint8_t* src;
};