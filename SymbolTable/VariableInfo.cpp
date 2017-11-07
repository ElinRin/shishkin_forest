#include "VariableInfo.h"
#include <string>

namespace SymbolTable {
	std::string VariableInfo::GetType() const
	{
		return type;
	}
	
	VariableInfo::VariableInfo( Position  _position, std::string _type ) :
		type( _type ),
		position( _position )
	{
	}
}