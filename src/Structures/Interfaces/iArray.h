#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iArray
    @ingroup Interfaces
    @brief interface for Procedural Memory inline Array
*/

#include "iDynamic.h"
#include "iCountable.h"
#include "iIndexable.h"
#include "iIterable.h"
#include "iQueue.h"

template <typename TYPE>
class iArray : public iCountable, public iIndexable<TYPE>, public iCIterable<TYPE>, public iSearchable<TYPE> {
public:
		

				
};

template <typename TYPE>
class iDArray : public iArray<TYPE>, public iDynamic,public iEditable<TYPE>, public iQueueDouble<TYPE> {
public:



};