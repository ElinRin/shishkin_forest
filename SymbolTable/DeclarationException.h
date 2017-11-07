#pragma once

#include <string>

namespace SymbolTable {
	class DeclarationException : public std::exception {
	public:
		virtual char const* what() const override;
		DeclarationException( const std::string& message );
	private:
		std::string msgresult;
	};
}