#pragma once

#include "VariableInfo.h"
#include "Position.h"
#include <vector>
#include <map>

namespace SymbolTable {

	class MethodInfo : public Symbol{
	public:
		MethodInfo( std::string _name, Position _position, std::string _returnType);
		int GetReturnType() const;
		void AddVariableInfo( const std::string name );
		const VariableInfo & GetVariableInfo( const std::string name ) const;
		void AddArgInfo( const std::string name );
		const VariableInfo & GetArgInfo( const std::string name  ) const;
		void addBlock( std::unordered_map<std::string, Symbol*> * _block );
		std::vector<std::string> GetArgsName();
		std::vector<std::string>  GetVarsName();
		int GetArgsCount();
		int GetVarsCount();

	private:
		std::string returnType;
		std::vector<std::string> varsName;
		std::vector<std::string> argsName;
		std::unordered_map<std::string, Symbol*> * block;
		Position position;
	};
}