//------------------------------------------------------------------------------
//  Entity.h
//------------------------------------------------------------------------------

#include "Schema.h"
#include "Structures/Containers/Table.h"

class Entity {
public:
	Entity(Schema* SchemaPointer) : schemaPtr(SchemaPointer) {};
private:
	Schema* schemaPtr;
	Table data;
};