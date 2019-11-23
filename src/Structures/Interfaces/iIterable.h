#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iIterable
    @ingroup Interfaces
    @brief interface for structures that can be Iterated
*/

template <typename TYPE>
class iCIterable {
public:
	/// C++ conform begin
	virtual TYPE* begin() = 0;
	/// C++ conform begin
	virtual const TYPE* begin() const = 0;
	/// C++ conform end
	virtual TYPE* end() = 0;
	/// C++ conform end
	virtual const TYPE* end() const = 0;
};