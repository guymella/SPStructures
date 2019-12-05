//------------------------------------------------------------------------------
//  KeyString.h
//------------------------------------------------------------------------------

#include "Structures/Containers/Array.h"

struct keyCompare {
	size_t commonPrefix = 0;
	size_t Postfix0 = 0;
	size_t Postfix1 = 0;
};

class iKeyString {
public:
	virtual bool operator==(const iKeyString& rhs) const;
	virtual keyCompare CompareTo(const iKeyString& rhs) const;
	virtual iArray<uint8_t>& StringData() = 0;
	virtual const iArray<uint8_t>& StringData() const = 0;
};

class iKeyRange{
	virtual bool operator==(const iKeyString& rhs) const = 0;
	virtual keyCompare CompareTo(const iKeyString& rhs) const = 0;
	virtual iKeyRange& Union(iKeyRange& rhs) = 0;
	virtual iKeyRange& Intersect(iKeyRange& rhs) = 0;
	//virtual iKeyRange& Subtract(iKeyRange& rhs) = 0;
};

class KeyString : public iKeyString {
public:
	virtual iArray<uint8_t>& StringData() override { return key; };
	virtual const iArray<uint8_t>& StringData() const override { return key; };
private:
	Array<uint8_t> key;
	size_t tare;
};

class KeyStringRef : public iKeyString {
public:
	virtual iArray<uint8_t>& StringData() override {return key;};
	virtual const iArray<uint8_t>& StringData() const override { return key; };
private:
	Slice<uint8_t> key;
};

bool iKeyString::operator==(const iKeyString& rhs) const
{
	keyCompare c = CompareTo(rhs);
	return !(c.Postfix0 || c.Postfix1);
}

inline keyCompare iKeyString::CompareTo(const iKeyString& rhs) const
{
	keyCompare c;
	const uint8_t* l = StringData().begin();
	const uint8_t* r = rhs.StringData().begin();
	size_t s = (rhs.StringData().Size() < StringData().Size()) ? rhs.StringData().Size() : StringData().Size();
	if (s)
		while (l[c.commonPrefix] == r[c.commonPrefix] && ++c.commonPrefix < s);
	c.Postfix0 = StringData().Size() - c.commonPrefix;
	c.Postfix1 = rhs.StringData().Size() - c.commonPrefix;
	return c;
}


