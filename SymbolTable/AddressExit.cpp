#pragma once

#include "Temp.h"
#include "AddressExit.h"

namespace SymbolTable {

	AddressExit::AddressExit(ActivationRecords::Temp _address) :
        address(_address)
    {
    }

}
