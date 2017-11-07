#include "Symbol.h"

namespace SymbolTable { 
  bool Symbol::operator ==( const Symbol& a, const Symbol& b )
	{
		return &a.String() == &b.String();
	}
	bool Symbol::operator !=( const Symbol& a, const Symbol& b )
	{
		return &a.String() != &b.String();
	}

}