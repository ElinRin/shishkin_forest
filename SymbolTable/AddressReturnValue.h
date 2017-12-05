#pragma once

#include "Temp.h"

namespace SymbolTable {

	class AddressReturnValue {
	public:
		AddressReturnValue(ActivationRecords::Temp _address);
	private:
		ActivationRecords::Temp address;
	};

}
