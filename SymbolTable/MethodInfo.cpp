#include "MethodInfo.h"
#include "VariableInfo.h"
#include "Position.h"
#include "DeclarationException.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace SymbolTable {

    MethodInfo::MethodInfo(const std::string &_name, const std::string &className, const Position _position, TypeInfo _returnType, T_Qualifier _qualifier) :
        className(StringSymbol::GetIntern(className)),
        Symbol(_name, _position),
        returnType(_returnType),
        qualifier(_qualifier)
		{	
		}

    const TypeInfo& MethodInfo::GetReturnType() const
	{
		return returnType;
	}

    void MethodInfo::AddVariableInfo( const VariableInfo* name )
	{
        auto declared = block.find(name->GetName());
        if(declared != block.end()) {
            throw DeclarationException("Variable redeclaration, " +
                                           declared->second->GetType().GetTypeString() + " " +
                                           name->GetName()->GetString() +
                                           " already defined at " + declared->second->GetPosition().ToString(),
                                           name->GetPosition());
        }
        varsNames.push_back(name);
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
        auto declared = block.find(name->GetName());
        if(declared != block.end()) {
            throw DeclarationException("Variable redeclaration, " +
                                           declared->second->GetType().GetTypeString() + " " +
                                           name->GetName()->GetString() +
                                           " already defined at " + declared->second->GetPosition().ToString(),
                                           name->GetPosition());
        }
        argsNames.push_back(name);
        block.insert(std::make_pair(name->GetName(), std::unique_ptr<const VariableInfo>(name)));
	}
	
    const std::vector<const VariableInfo*> &MethodInfo::GetArgsNames() const
	{
        return argsNames;
	}

    const std::vector<const VariableInfo*>& MethodInfo::GetVarsName() const
	{
        return varsNames;
	}

    int MethodInfo::GetArgsCount() const
	{
        return argsNames.size();
	}

    int MethodInfo::GetVarsCount() const
	{
        return varsNames.size();
	}
}
