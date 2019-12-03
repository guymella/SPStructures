#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iDynamic
    @ingroup Interfaces
    @brief interface for linear structures with dynamic size, and preallocated free space
*/

#include "Structures/Types.h"

struct CopyRange {
	size_t dstOffset;
	void* src;
	size_t size;
	};


	class iDynamicCap{
	public:
		virtual size_t Capacity() const = 0;
		virtual size_t Spare() const = 0; //
	};

	class iDynamicFlatten {
	public:
		virtual void Trim() = 0; // remove all spare
		virtual void Clear() = 0;
	};

	class iDynamicGrow {
	public:
		virtual void Grow() = 0; // just grow (very dumb grow = golden ratio)
		virtual void Grow(const size_t& newSize) = 0; //grow to specific size, copy all to front (dumb grow)
		virtual void Grow(const size_t& newSize, const size_t& frontPorch) = 0; //grow to specific size, copy all to front plus offsaet (dumb grow)
		//virtual void Grow(const size_t& newSize, CopyRange* CopyMap, size_t CopyMapSize) = 0; //grow to allocation block size factor, copy by map (smart grow)
	};

	class iDynamicFront {
	public:
		virtual size_t SpareFront() const = 0;
		virtual void ReserveFront(const size_t& numElements) = 0;	
	};

	class iDynamicBack{
	public:
		virtual size_t SpareBack() const = 0;
		virtual void ReserveBack(const size_t& numElements) = 0;
	};

	class iDynamic : public iDynamicCap, public iDynamicFront, public iDynamicBack, public iDynamicFlatten, public iDynamicGrow {
	public:
		virtual void Reserve(const size_t& numElementsBack) = 0;
		virtual void Reserve(const size_t& numElementsFront, const size_t& numElementsBack) = 0;
		virtual void ShiftBack(const size_t& numShift) = 0; //free up spare without realloc
		virtual void ShiftFront(const size_t& numShift) = 0;
		virtual void ShiftRange(size_t StartIndex, size_t numElements, const int64_t& shiftAmmount) =0;
	};