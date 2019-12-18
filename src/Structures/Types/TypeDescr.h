//------------------------------------------------------------------------------
//  VarInt.h
//------------------------------------------------------------------------------

#include "Structures/Types.h"
#include "KeyString.h"

#ifndef Included_TypeDescr_H
#define Included_TypeDescr_H
namespace Types {
	enum TypeSequence : uint8_t {
		Fixed = 0,
		Cached = 1,
		FixedMulti = 2,
		CachedFixedMulti = 3,

		Schema = 8,
		CachedSchema = 9,
		FixedMultiSchema = 10,
		CachedFixedMultiSchema = 11,

		Nullable = 12,
		CachedNullable = 13,
		NullableFixedMulti = 14,
		CachedNullableFixedMulti = 15,

		NullableSchema = 20,
		CachedNullableSchema = 21,
		NullableFixedMultiSchema = 22,
		CachedNullableFixedMultiSchema = 23,

		Sparse = 24,
		CachedSparse = 25,
		SparseFixedMulti = 26,
		CachedSparseFixedMulti = 27,

		SparseSchema = 20,
		CachedSparseSchema = 21,
		SparseFixedMultiSchema = 22,
		CachedSparseFixedMultiSchema = 23,

		Multi = 24,
		CachedMulti = 25,
		MultiSchema = 26,
		CachedMultiSchema = 27,

		Ref = 4,
		CachedRef = 5,
		MultiRef = 26,
		CachedMultiRef = 22,
		NullableRef = 16,
		CachedNullableRef = 17,
		NullableMultiRef = 18,
		CachedNullableMultiRef = 19,

		Uncached = 28,
		UncachedMulti = 29,
		UncachedSchema = 30,
		UncachedMultiSchema = 31,
		UcachedRef = 28,
		UncachedMultRef = 29,

		UncachedNullable = 32,
		UncachedNullableMulti = 33,
		UncachedNullableSchema = 34,
		UncachedNullableMultiSchema = 35,
		UncachedNullableRef = 22,
		UncachedNullableMultiRef = 23
	};

	class Constraint {
	public:
		bool Contains(const KeyString& key) const;
		bool operator==(const Constraint& rhs) const { return true; };
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

	struct TypeDescr {
		baseTypes type = baseTypes::Void;
		uint8_t flags = 0;
		bool operator==(const TypeDescr& rhs) const;
		bool Nullable() const { return (flags) & 1U; };
		bool Multiple() const { return (flags >> 1) & 1U; };
		bool Sparse() const { return (flags >> 2) & 1U; };
		//bool CustomDefault() const { return (flags >> 3) & 1U; };
		bool Constrained() const { return (flags >> 4) & 1U; };
		bool Derived() const { return (flags >> 5) & 1U; };
		bool Cached() const { return (flags >> 6) & 1U; };
		bool Subscribable() const { return (flags >> 7) & 1U; };
		void Nullable(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL); };
		void Multiple(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 1); };
		void Sparse(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 2); };
		//void CustomDefault(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 3); };
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
			if (!Multiple() || (constraint && constraint->Contains("FixedSize"))) {
				if (!Nullable() && !Sparse()) {
					if (!(type == baseTypes::Struct || type == baseTypes::VarStruct)) {
						if (!Multiple())
							return Cached() ? TypeSequence::Cached : Fixed;
						else
							return Cached() ? CachedFixedMulti : FixedMulti;
					}
					else { //struct
						if (!Multiple())
							return Cached() ? CachedSchema : Schema;
						else
							return Cached() ? CachedFixedMultiSchema : FixedMultiSchema;
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
				if (type == baseTypes::SRef || type == baseTypes::PRef || type == baseTypes::LRef) { //ref
					if (!Nullable()) {
						if (!Multiple())
							return Cached() ? CachedRef : Ref;
						else
							Cached() ? CachedMultiRef : MultiRef;
					}else {
						if (!Multiple())
							Cached() ? CachedNullableRef : NullableRef;
						else
							Cached() ? CachedNullableMultiRef : NullableMultiRef;
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
				else if (type == baseTypes::SRef || type == baseTypes::PRef || type == baseTypes::LRef)
					return Multiple() ? UncachedMultRef : UcachedRef;
				else
					return Multiple() ? UncachedMulti : Uncached;
			} else {
				if (type == Struct || type == VarStruct)
					return Multiple() ? UncachedNullableMultiSchema : UncachedNullableSchema;
				else if (type == baseTypes::SRef || type == baseTypes::PRef || type == baseTypes::LRef)
					return Multiple() ? UncachedNullableMultiRef : UncachedNullableRef;
				else
					return Multiple() ? UncachedNullableMulti : UncachedNullable;
			}
		}
		return TypeSequence::Uncached;
	}
};
#endif 