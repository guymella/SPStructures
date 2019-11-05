#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iProcArray
    @ingroup Interfaces
    @brief interface for Procedural Memory
*/
#include <stdint.h>

	class iProcArray{
	public:
		iProcArray(iProcArray* ParentArray);

		//iProcArray GetSubArray(size_t index);

		//template <typename T> T GetValue(size_t index);
		virtual size_t Size() const = 0;
	private:
		
	};
