//------------------------------------------------------------------------------
//  VarInt.h
//------------------------------------------------------------------------------

#include "TypeDescr.h"

#include "Structures/Containers/SparseArray.h"



class Schema {
public:
	bool AddAttribute(const iKeyString& name, Types::TypeDescr type);
	bool AddAttribute(const char* name, baseTypes type);	
private:	
	bool Extendable;
	SparseArray<size_t> index;
	Array<KeyString> Labels;
	Array<Types::TypeDescr> Types;
	//SparseArray<Darivation> Darivations
	//SparseArray<Constraint> Constraints;
	//SparseArray<Schema*> SubSchemas;
};





inline bool Schema::AddAttribute(const iKeyString& name, Types::TypeDescr type)
{
	for (size_t i = 0; i < Labels.Size(); i++) {
		if (Labels[i] == name)
			return false;//attribute name already used
	}

	Labels.PushBack(name);
	Types.PushBack(type);

	return true;
}

inline bool Schema::AddAttribute(const char* name, baseTypes type)
{
	Types::TypeDescr t;
	t.type = type;
	return AddAttribute(KeyString(name), t);
}

