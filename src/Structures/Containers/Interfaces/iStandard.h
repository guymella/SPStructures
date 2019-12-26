#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iStandard
    @ingroup Interfaces
    @brief Standar interfaces for all structures
*/

	template <typename TYPE>
	class iCopyable{
	public:
		/// copy-assignment operator (truncates to actual size)
		virtual void operator=(const TYPE& rhs);
		/// move-assignment operator (same capacity and size)
		virtual void operator=(TYPE&& rhs);
	private:
		virtual void copy(const TYPE& rhs) = 0;
		virtual void move(TYPE&& rhs) = 0;
	};

	template <typename TYPE>
	class iEquality{
	public:
		/// copy-assignment operator (truncates to actual size)
		virtual bool operator==(const TYPE& rhs) {
			return equals(rhs);
		};
	private:
		virtual bool equals(const TYPE& rhs) = 0;
	};

	template <typename TYPE>
	class iComparable {
	public:
		/// copy-assignment operator (truncates to actual size)
		virtual bool operator<(const TYPE& rhs) {
			return lessthen(rhs);
		};
		virtual bool operator>(const TYPE& rhs) {
			return greaterthen(rhs);
		};
	private:
		virtual bool lessthen(const TYPE& rhs) = 0;
		virtual bool greaterthen(const TYPE& rhs) = 0;
	};

	template<typename TYPE>
	inline void iCopyable<TYPE>::operator=(const TYPE& rhs)
	{
		copy(rhs);
	}
	template<typename TYPE>
	inline void iCopyable<TYPE>::operator=(TYPE&& rhs)
	{
		move(rhs);
	};
	;
