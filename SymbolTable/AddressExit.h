#pragma once

#include <string>
#include "Temp.h"

namespace SymbolTable {

	class AddressExit {
	public:
		AddressExit(ActivationRecords::Temp _address);
	private:
		ActivationRecords::Temp address;
	};

}
