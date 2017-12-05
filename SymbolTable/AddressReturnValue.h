#pragma once

#include <string>
#include "Temp.h"

namespace SymbolTable {

	class AddressReturnValue {
	public:
		AddressReturnValue(ActivationRecords::Temp _address);
		std::string ToString() const;
	private:
		ActivationRecords::Temp address;
	};
	
}
