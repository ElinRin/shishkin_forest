#include "DeclarationException.h"

namespace SymbolTable {

	char const * DeclarationException::what() const
	{
		return msgresult.c_str();
	}

	DeclarationException::DeclarationException( const std::string & message )
	{
		msgresult+= message;
	}
}
