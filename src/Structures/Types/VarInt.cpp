#include "VarInt.h"

long long VarUInt::Size()
{
	if (src[0] < 127) return src[0];		
}
