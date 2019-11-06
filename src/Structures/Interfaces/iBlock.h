#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iBlock
    @ingroup Interfaces
    @brief interface for Procedural Memory Block
	
*/


	class iBlock{
	public:
		iBlock() {};
		virtual size_t Size() const = 0;
		virtual void* memStart() = 0; 
		void Grow(); // just grow (very dumb grow = golden ratio)
		void Grow(const size_t& newSize); //grow to specific size, copy all to front (dumb grow)
		void Grow(const size_t& SizeFactor, size_t* CopyMap); //grow to allocation block size factor, copy by map (smart grow)
	};
