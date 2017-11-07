#include "Symbol.h"

namespace SymbolTable { 
	
	const std::string& Symbol::String() const 
	{
		return name;
	}

  bool Symbol::operator ==( const Symbol& a )
	{
		return &a == this;
	}
	bool Symbol::operator !=( const Symbol& a )
	{
		return &a != this;
	}

}