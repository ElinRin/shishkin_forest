#pragma once

#include "VariableInfo.h"
#include "Position.h"
#include <vector>
#include <map>

namespace SymbolTable {

	class MethodInfo : public Symbol{
	public:
		MethodInfo( Position position, std::string returnType);
		int GetReturnType() const;
		void AddVariableInfo( const std::string name, const VariableInfo * info );
		const VariableInfo & GetVariableInfo( const std::string name ) const;
		void AddArgInfo( const std::string name, const VariableInfo * info );
		const VariableInfo & GetArgInfo( const std::string name  ) const;
		std::vector<std::string> GetArgsName();
		std::vector<std::string>  GetVarsName();
		int GetArgsCount();
		int GetVarsCount();

	private:
		std::string returnType;
		std::map<int, VariableInfo> vars;
		std::map<int, VariableInfo> args;
	};
}