#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iBlock
    @ingroup Interfaces
    @brief interface for Procedural Memory Block
	
*/
#include "Structures/Types.h"
#include "iDynamic.h"
#include "iCountable.h"



class iBlock : public iCountable {
public:
	virtual void* memStart() = 0;
	virtual const void* memStart() const = 0;

	void* headerStart();
	void* headerStart() const;
	baseTypes Type() const;
	virtual void SetType(baseTypes newType);
};
   

class iDBlock : public iBlock, public iDynamicGrow {
public:
	void SetType(baseTypes newType) override;		
};


inline void* iBlock::headerStart() {
	return (baseTypes*)memStart() + 1;
}

inline void* iBlock::headerStart() const {
	return (baseTypes*)memStart() + 1;
}

inline baseTypes iBlock::Type() const {
	if (Size()) {
		return *((baseTypes*)memStart());
	}
	return baseTypes::Void;
}

inline void iBlock::SetType(baseTypes newType) {
	*((baseTypes*)memStart()) = newType;
}

inline void iDBlock::SetType(baseTypes newType) {
	if (!Size()) {
		Grow(1);
	}
	iBlock::SetType(newType);
}
