#pragma once

#include "Temp.h"
#include "AddressReturnValue.h"

namespace SymbolTable {

	AddressReturnValue::AddressReturnValue(ActivationRecords::Temp _address) :
        address(_address)
    {
    }

}
