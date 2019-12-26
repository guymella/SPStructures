#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iCountable
    @ingroup Interfaces
    @brief interface for structures that have countable size
*/


class iCountable {
public:
	virtual size_t Size() const = 0;
	virtual bool Empty() const { return !Size(); };
};

//class iBlockCountable { //for blocks always in bytes
//public:
//	virtual size_t MemSize() const = 0;
//	virtual bool MemEmpty() const { return !MemSize(); };
//};
