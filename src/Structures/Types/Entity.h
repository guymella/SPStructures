//------------------------------------------------------------------------------
//  Entity.h
//------------------------------------------------------------------------------

#include "Schema.h"
#include "Structures/Containers/Table.h"

class Entity {
public:
	Entity(Schema* SchemaPointer);
private:
	Schema* schemaPtr;
	Table data;
};

Entity::Entity(Schema* SchemaPointer) : schemaPtr(SchemaPointer)
{
	//data.GetIndex() = schemaPtr->GetTableIndex();
}