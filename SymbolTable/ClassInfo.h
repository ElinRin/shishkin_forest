#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "MethodInfo.h"
#include "VariableInfo.h"
#include "Symbol.h"

namespace SymbolTable {

	class ClassInfo : public Symbol {
	public:
		ClassInfo( std::string _name, Position _position );
		void AddMethodInfo( const std::string name );
		void AddVariableInfo( const std::string name );
		const VariableInfo& GetVariableInfo( const std::string name ) const;
		const MethodInfo& GetMethodInfo( const std::string name ) const;
		const std::vector<MethodInfo>& GetMethods() const;
		void addBlock( std::unordered_map<std::string, Symbol*> * block );
		std::vector<std::string> GetMethodName();
		std::vector<std::string>  GetVarsName();
		int GetMethodCount();
		int GetVarsCount();

	private:
		std::vector< std::string > methodsName;
		std::vector< std::string > varsName;
		std::unordered_map< std::string, Symbol* > * block;
		Position position;
    std::string name;
	};
}