#include "DeclarationException.h"

namespace SymbolTable {

	char const * DeclarationException::what() const noexcept
	{
		return msgresult.c_str();
	}

    DeclarationException::DeclarationException( const std::string& message,
                                                const Position& position)
	{
        msgresult += position.ToString();
		msgresult += message;
	}
}
