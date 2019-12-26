#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iDynamic
    @ingroup Interfaces
    @brief interface for linear structures with dynamic size, and preallocated free space
*/

#include "Structures/Types/CopyMap.h"



	class iDynamicCap{
	public:
		//Total Capacity of container without reallocation
		virtual size_t Capacity() const = 0;
		/// total Amount of additional ellements that can be added without reallocation
		virtual size_t Spare() const = 0; //
	};

	class iDynamicFlatten {
	public:
		// remove all spare reallocate to smallest cappacity that fits current size
		virtual void Trim() = 0;
	};

	class iGrowable {
	public:
		// Grow Automatically
		virtual inline void Grow() = 0; 
		//grow to specific size, copy all to front
		virtual void Grow(const size_t& newSize) = 0; 
		//grow to specific size, copy all to front with offset
		virtual void Grow(const size_t& newSize, const size_t& offset) = 0; 
	};

	class iSmartGrowable {
	public:
		//grow using CopyMap
		virtual void GrowMap(const CopyMap& map) = 0; 
	};

	class iDynamicFront {
	public:
		/// total number of additional aelements that can be added to front without reallocation
		virtual size_t SpareFront() const = 0;
		/// realocate and include At least numelelents Spare at front
		virtual void ReserveFront(const size_t& numElements) = 0;	
	};

	class iDynamicBack{
	public:
		/// total number of additional elements that can be added to back without reallocation
		virtual size_t SpareBack() const = 0;
		/// realocat and include at least numElements Spare At Back
		virtual void ReserveBack(const size_t& numElements) = 0;
	};

	class iDynamic : public iDynamicCap, public iDynamicFront, public iDynamicBack, public iDynamicFlatten, public iGrowable {
	public:
		/// Realocate and Reserve At least numElements additional elelemnts at Back 
		virtual void Reserve(const size_t& numElementsBack) = 0;
		// Realocate and Reserve At front and back
		virtual void Reserve(const size_t& numElementsFront, const size_t& numElementsBack) = 0;
		//Move Spare from back To Front without reallocation
		virtual void ShiftBack(const size_t& numShift) = 0; 
		//Move Spare from Front to Back without Reallocation
		virtual void ShiftFront(const size_t& numShift) = 0;
	};