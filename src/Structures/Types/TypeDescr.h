//------------------------------------------------------------------------------
//  VarInt.h
//------------------------------------------------------------------------------

#include "Structures/Types.h"


struct TypeDescr {
	baseTypes type = baseTypes::Void;
	uint8_t flags = 0;
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
};

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