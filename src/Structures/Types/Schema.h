//------------------------------------------------------------------------------
//  VarInt.h
//------------------------------------------------------------------------------

#include "TypeDescr.h"
#include "KeyString.h"
#include "Structures/Containers/SparseArray.h"

class Constraint {
public:
	bool Contains(const KeyString& key) const;
	bool operator==(const Constraint& rhs) const {	return true ;	};
private:
	Array<KeyString> names;
	Array<Array<uint8_t>> args;
};

bool Constraint::Contains(const KeyString& key) const
{
	for (size_t i = 0; i < names.Size(); i++)
		if (names[i] == key)
			return true;
	return false;
}

class Schema {
public:
	bool AddAttribute(const iKeyString& name, TypeDescr type);
	bool AddAttribute(const char* name, baseTypes type);

	void Normalize();
	bool Normalized() const;
private:	
	static inline bool IsFlat(const TypeDescr& t) { return !t.Derived() && !t.Sparse() && !t.Multiple(); };
	static inline bool IsFixedMulti(const TypeDescr& t, const Constraint& c) { return !t.Derived() && t.Multiple() && c.Contains("FixedSize"); };
	static inline bool IsCached(const TypeDescr& t) { return t.Derived() && t.Cached() && !t.Sparse() && !t.Multiple(); };
	static inline bool IsCachedFixedMulti(const TypeDescr& t, const Constraint& c) { return t.Derived() && t.Cached() && t.Multiple() && c.Contains("FixedSize"); };
	static inline bool IsCachedSparce(const TypeDescr& t) { return t.Derived() && t.Cached() && t.Sparse(); };
	static inline bool IsCachedMulti(const TypeDescr& t, const Constraint& c) { return  t.Derived() && t.Cached() && t.Multiple() && !c.Contains("FixedSize");; };
	static inline bool IsUncached(const TypeDescr& t) { return t.Derived() && !t.Cached(); };
	static inline bool IsSparce(const TypeDescr& t) { return !t.Derived() && t.Sparse(); };
	static inline bool IsMulti(const TypeDescr& t, const Constraint& c) { return !t.Derived() && t.Multiple() && !c.Contains("FixedSize"); };

	bool Extendable;
	size_t flatColumns = 0;
	size_t fixedMultiColumns = 0;
	size_t cachedDerivedColumns = 0; 
	size_t cachedDerivedFixedMultiColumns = 0;
	size_t cachedDerivedSparseCcolumns = 0;
	size_t cachedDerivedMultiColumns = 0;
	size_t derivedColumns = 0;
	size_t sparseColumns = 0;
	size_t multiColumns = 0;
	Array<KeyString> Labels;
	Array<TypeDescr> Types;
	//SparseArray<Darivation> Darivations
	SparseArray<Constraint> Constraints;
};





inline bool Schema::AddAttribute(const iKeyString& name, TypeDescr type)
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
	TypeDescr t;
	t.type = type;
	return AddAttribute(KeyString(name), t);
}

inline void Schema::Normalize()
{
	#define filt(columnName,filtername) \
			for (size_t i = Types.Size() - 1; true; ) \
			{ \
				while (filtername(Types[columnName])) \
					columnName++; \
				while (!filtername(Types[i]) && i > columnName) \
					i--; \
				if (i > columnName) { \
					Labels.Swap(columnName, i); \
					Types.Swap(columnName, i); \
				} \
				else \
					break; \
			}

	#define filtConst(columnName,filtername) \
		for (size_t i = Types.Size()-1; true;) \
		{ \
			while (filtername(Types[columnName],Constraints[columnName]))  \
				columnName++; \
			while (!filtername(Types[i],Constraints[i]) && i > columnName) \
				i--; \
			if (i > columnName) {	\
					Labels.Swap(columnName, i); \
					Types.Swap(columnName, i); \
			}\
			else\
				break; \
		}

	flatColumns = 0;
	filt(flatColumns, IsFlat);

	fixedMultiColumns = flatColumns; 
	filtConst(fixedMultiColumns, IsFixedMulti);

	cachedDerivedColumns = fixedMultiColumns;
	filt(cachedDerivedColumns, IsCached);

	cachedDerivedFixedMultiColumns = cachedDerivedColumns;
	filtConst(cachedDerivedFixedMultiColumns, IsCachedFixedMulti);

	cachedDerivedSparseCcolumns = cachedDerivedFixedMultiColumns;
	filt(cachedDerivedSparseCcolumns, IsCachedSparce);

	cachedDerivedMultiColumns = cachedDerivedSparseCcolumns;
	filtConst(cachedDerivedMultiColumns, IsCachedMulti);

	derivedColumns = cachedDerivedMultiColumns;
	filt(derivedColumns, IsUncached);

	sparseColumns = derivedColumns;
	filt(sparseColumns, IsSparce);

	multiColumns = sparseColumns;
	filtConst(multiColumns, IsMulti);

	//multiColumns -= sparseColumns;
	//sparseColumns -= derivedColumns;
	//derivedColumns -= cachedDerivedMultiColumns;
	//cachedDerivedMultiColumns -= cachedDerivedSparseCcolumns;
	//cachedDerivedSparseCcolumns -= cachedDerivedFixedMultiColumns;
	//cachedDerivedFixedMultiColumns -= cachedDerivedColumns;
	//cachedDerivedColumns -= fixedMultiColumns;
	//fixedMultiColumns -= flatColumns;

}

bool Schema::Normalized() const
{
	/*return (flatColumns +
		fixedMultiColumns +
		cachedDerivedColumns +
		cachedDerivedSparseCcolumns +
		cachedDerivedMultiColumns +
		derivedColumns +
		sparseColumns +
		multiColumns) == Types.Size();*/

	return multiColumns == Types.Size();
}

