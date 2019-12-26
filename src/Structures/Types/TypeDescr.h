//------------------------------------------------------------------------------
//  VarInt.h
//------------------------------------------------------------------------------

#include "Structures/Types.h"
#include "KeyString.h"

#ifndef Included_TypeDescr_H
#define Included_TypeDescr_H
namespace Types {
	enum TypeSequence : uint8_t {
		Bool,
		CachedBool,
		FixedMultiBool,
		CachedFixedMultiBool,

		Fixed,
		Cached,
		FixedMulti,
		CachedFixedMulti,

		Schema,
		CachedSchema,
		FixedMultiSchema,
		CachedFixedMultiSchema,

		Nullable,
		CachedNullable,
		NullableFixedMulti,
		CachedNullableFixedMulti,

		NullableSchema,
		CachedNullableSchema,
		NullableFixedMultiSchema,
		CachedNullableFixedMultiSchema,

		Sparse,
		CachedSparse,
		SparseFixedMulti,
		CachedSparseFixedMulti,

		SparseSchema,
		CachedSparseSchema,
		SparseFixedMultiSchema,
		CachedSparseFixedMultiSchema,

		Columnar,
		CachedColumnar,

		Multi,
		CachedMulti,
		MultiSchema,
		CachedMultiSchema,

		Var,
		CachedVar,		
		NullableVar,
		CachedNullableVar,

		MultiVar,
		CachedMultiVar,
		NullableMultiVar,
		CachedNullableMultiVar,

		Uncached,
		UncachedMulti,
		UncachedSchema,
		UncachedMultiSchema,
		UcachedVar,
		UncachedMultVar,

		UncachedNullable,
		UncachedNullableMulti,
		UncachedNullableSchema,
		UncachedNullableMultiSchema,
		UncachedNullableVar,
		UncachedNullableMultiVar
	};

	class Constraint {
	public:
		iArray<uint8_t>* Contains(const KeyString& key) const;
		bool operator==(const Constraint& rhs) const { return true; };
	private:
		Array<KeyString> names;
		Array<Array<uint8_t>> args;
	};

	iArray<uint8_t>* Constraint::Contains(const KeyString& key) const
	{
		for (size_t i = 0; i < names.Size(); i++)
			if (names[i] == key)
				return (iArray<uint8_t> * )args.begin(i).Ptr();
		return NULL;
	}

	struct TypeDescr {
		baseTypes type = baseTypes::Void;
		uint8_t flags = 0;
		bool operator==(const TypeDescr& rhs) const;
		bool Nullable() const { return (flags) & 1U; };
		bool Multiple() const { return (flags >> 1) & 1U; };
		bool Sparse() const { return (flags >> 2) & 1U; };
		bool Columnar() const { return (flags >> 3) & 1U; };
		bool Constrained() const { return (flags >> 4) & 1U; };
		bool Derived() const { return (flags >> 5) & 1U; };
		bool Cached() const { return (flags >> 6) & 1U; };
		bool Subscribable() const { return (flags >> 7) & 1U; };
		void Nullable(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL); };
		void Multiple(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 1); };
		void Sparse(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 2); };
		void Columnar(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 3); };
		void Constrained(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 4); };
		void Derived(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 5); };
		void Cached(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 6); };
		void Subscribable(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 7); };

		static uint8_t setFLags(bool nullable, bool Multiple, bool Sparse, bool Constrained, bool Derived, bool Cached, bool Subscribable);
		TypeSequence getTypeSequence(const Constraint* constraint = NULL) const;
	};

	inline bool TypeDescr::operator==(const TypeDescr& rhs) const
	{
		return (type == rhs.type && flags == rhs.flags);
	}

	uint8_t TypeDescr::setFLags(bool nullable, bool Multiple, bool Sparse, bool Constrained, bool Derived, bool Cached, bool Subscribable)
	{
		return ((uint8_t)nullable)
			| ((uint8_t)Multiple << 1)
			| ((uint8_t)Sparse << 2)
			| ((uint8_t)Constrained << 4)
			| ((uint8_t)Derived << 5)
			| ((uint8_t)Cached << 6)
			| ((uint8_t)Multiple << 7);
	}

	inline TypeSequence TypeDescr::getTypeSequence(const Constraint* constraint) const
	{
		if (!Derived() || Cached()) {
			if ((!Multiple() && (type <= baseTypes::Struct)) || (constraint && constraint->Contains("FixedSize"))) {
				if (!Nullable() && !Sparse()) {
					if ((type == baseTypes::Struct || type == baseTypes::VarStruct)) {
						if (!Multiple())
							return Cached() ? CachedSchema : Schema;
						else
							return Cached() ? CachedFixedMultiSchema : FixedMultiSchema;
					}
					else if (type == baseTypes::boolean) {
						if (!Multiple())
							return Cached() ? CachedBool : Bool;
						else
							return Cached() ? CachedFixedMultiBool : FixedMultiBool;
					}
					else { //struct
						if (!Multiple())
							return Cached() ? TypeSequence::Cached : Fixed;
						else
							return Cached() ? CachedFixedMulti : FixedMulti;
						
					}
				}
				else if (Nullable() && !Sparse()) {
					if (!(type == baseTypes::Struct || type == baseTypes::VarStruct)) {
						if (!Multiple())
							return Cached() ? CachedNullable : TypeSequence::Nullable;
						else
							return Cached() ? CachedNullableFixedMulti : NullableFixedMulti;
					}
					else { //struct
						if (!Multiple())
							return Cached() ? CachedNullableSchema : NullableSchema;
						else
							return Cached() ? CachedNullableFixedMultiSchema : NullableFixedMultiSchema;
					}
				}
				else { //Sparce
					if (!(type == baseTypes::Struct || type == baseTypes::VarStruct)) {
						if (!Multiple())
							return Cached() ? CachedSparse : TypeSequence::Sparse;
						else
							return Cached() ? CachedSparseFixedMulti : SparseFixedMulti;
					}
					else { //struct
						if (!Multiple())
							return Cached() ? CachedSparseSchema : SparseSchema;
						else
							return Cached() ? CachedSparseFixedMultiSchema : SparseFixedMultiSchema;
					}
				}
			} else { //Not Fixed Size
				if (type > baseTypes::Struct || type <= baseTypes::LRef) { //var
					if (!Nullable()) {
						if (!Multiple())
							return Cached() ? CachedVar : Var;
						else
							return Cached() ? CachedMultiVar : MultiVar;
					}else {
						if (!Multiple())
							return Cached() ? CachedNullableVar : NullableVar;
						else
							return Cached() ? CachedNullableMultiVar : NullableMultiVar;
					}
				}
				else {
					if (!(type == baseTypes::Struct || type == baseTypes::VarStruct))
						return Cached() ? CachedMulti : Multi;
					else
						return Cached() ? CachedMultiSchema : MultiSchema;
				}					
			}
		}
		else {//unCached
			if (!Nullable()) {
				if (type == Struct || type == VarStruct)
					return Multiple() ? UncachedMultiSchema : UncachedSchema;
				else if (type > baseTypes::Struct || type <= baseTypes::LRef)
					return Multiple() ? UncachedMultVar : UcachedVar;
				else
					return Multiple() ? UncachedMulti : Uncached;
			} else {
				if (type == Struct || type == VarStruct)
					return Multiple() ? UncachedNullableMultiSchema : UncachedNullableSchema;
				else if (type > baseTypes::Struct || type <= baseTypes::LRef)
					return Multiple() ? UncachedNullableMultiVar : UncachedNullableVar;
				else
					return Multiple() ? UncachedNullableMulti : UncachedNullable;
			}
		}
		return TypeSequence::Uncached;
	}
};
#endif 