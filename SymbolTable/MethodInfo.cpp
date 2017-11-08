#include "MethodInfo.h"
#include "VariableInfo.h"
#include "Position.h"
#include "DeclarationException.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace SymbolTable {

    MethodInfo::MethodInfo(std::string _name, Position _position, TypeInfo _returnType, T_Qualifier _qualifier) :
        Symbol(_name, _position),
        returnType(_returnType),
        qualifier(_qualifier)
		{	
		}

    TypeInfo MethodInfo::GetReturnType() const
	{
		return returnType;
	}

    void MethodInfo::AddVariableInfo( const VariableInfo* name )
	{
        varsName.push_back(name->GetName());
        block.insert(std::make_pair(name->GetName(), std::unique_ptr<const VariableInfo>(name)));
	}

    const VariableInfo* MethodInfo::GetVariableInfo( const StringSymbol* name, const Position& position ) const
	{
        const VariableInfo* variable = block.find(name)->second.get();
        if(variable == nullptr) {
            throw DeclarationException("Variable " + name->GetString() +
                                           " in class " + this->name->GetString() + "undeclared", position);
        }
        return variable;
	}

    void MethodInfo::AddArgInfo( const VariableInfo* name )
	{
        argsName.push_back(name->GetName());
        block.insert(std::make_pair(name->GetName(), std::unique_ptr<const VariableInfo>(name)));
	}
	
    const std::vector<const StringSymbol*>& MethodInfo::GetArgsName() const
	{
		return argsName;
	}

    const std::vector<const StringSymbol*>& MethodInfo::GetVarsName() const
	{
		return varsName;
	}

	int MethodInfo::GetArgsCount()
	{
		return argsName.size();
	}

	int MethodInfo::GetVarsCount()
	{
		return varsName.size();
	}
}
