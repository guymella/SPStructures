//------------------------------------------------------------------------------
//  VarInt.h
//------------------------------------------------------------------------------

#include "Schema.h"
#include "Structures/Containers/Table.h"

class Object {
public:
	Object(Schema* SchemaPointer) : schemaPtr(SchemaPointer) {};
private:
	Schema* schemaPtr;
	Table data;
};