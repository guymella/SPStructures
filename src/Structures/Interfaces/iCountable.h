#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iCountable
    @ingroup Interfaces
    @brief interface for structures that have countable size
*/


	class iCountable{
	public:
		virtual size_t Size() const = 0;
		bool Empty() const { return !Size(); };				
	};
