#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iBlock
    @ingroup Interfaces
    @brief interface for Procedural Memory Block
	
*/
#include "Structures/Types.h"

	class iBlock{
	public:
		virtual size_t Size() const = 0;
		virtual void* memStart() = 0;
		virtual const void* memStart() const = 0;
		virtual void Grow() = 0; // just grow (very dumb grow = golden ratio)
		virtual void Grow(const size_t& newSize) = 0; //grow to specific size, copy all to front (dumb grow)
		//void Grow(const size_t& SizeFactor, size_t* CopyMap); //grow to allocation block size factor, copy by map (smart grow)
		
		void* headerStart();
		void* headerStart() const;
		baseTypes Type() const;
		void SetType(baseTypes newType);
		
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
		if (!Size()) {
			Grow(1);
		}
		*((baseTypes*)memStart()) = newType;
	}
