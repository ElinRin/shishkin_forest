#pragma once

#include "MethodInfo.h"
#include "VariableInfo.h"
#include "Position.h"
#include <vector>
#include <map>

namespace SymbolTable {

	int MethodInfo::GetReturnType() const
	{
		return returnType;
	}
}