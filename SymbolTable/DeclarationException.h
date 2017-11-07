#pragma once

#include <string>

namespace SymbolTable {
	class DeclarationException : public std::exception {
	public:
		virtual char const* what() const noexcept override;
		DeclarationException( const std::string& message );
		virtual ~DeclarationException() throw() {}
	private:
		std::string msgresult;
	};
}
