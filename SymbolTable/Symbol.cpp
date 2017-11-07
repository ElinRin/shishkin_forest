#include "Symbol.h"

namespace SymbolTable { 
	
	const std::string& Symbol::String() const 
	{
		return name;
	}

  bool Symbol::operator ==( const Symbol& a, const Symbol& b )
	{
		return &a.String() == &b.String();
	}
	bool Symbol::operator !=( const Symbol& a, const Symbol& b )
	{
		return &a.String() != &b.String();
	}

}