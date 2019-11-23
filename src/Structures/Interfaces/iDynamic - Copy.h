#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iDynamic
    @ingroup Interfaces
    @brief interface for linear structures with dynamic size, and preallocated free space
*/


	class iDynamicCap{
	public:
		virtual size_t Capacity() const = 0;
		virtual void Trim() = 0; // remove all spare				
	};

	class iDynamicFlatten {
	public:
		virtual void Trim() = 0; // remove all spare				
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

	class iDynamic : public iDynamicCap, public iDynamicFront, public iDynamicBack, public iDynamicFlatten {
	public:
		virtual void Reserve(const size_t& numElementsBack) = 0;
		virtual void Reserve(const size_t& numElementsFront, const size_t& numElementsBack) = 0;					
	};