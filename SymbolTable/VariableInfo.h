#pragma once

namespace SymbolTable {

	class VariableInfo : public Symbol {
	public:
		std::string GetType() const;
		VariableInfo( std::string _name, Position  _position, std::string _type );
	private:
		std::string type;
		Position position;
	};
}