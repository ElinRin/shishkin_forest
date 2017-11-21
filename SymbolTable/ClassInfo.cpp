#include <vector>
#include <unordered_map>
#include "MethodInfo.h"
#include "VariableInfo.h"
#include "ClassInfo.h"
#include "Symbol.h"
#include "DeclarationException.h"

namespace SymbolTable {

ClassInfo::ClassInfo(const std::string _name, const Position _position ) :
    Symbol(_name,  _position),
    superClass(nullptr),
    info(VT_UserClass, _name)
{}

void ClassInfo::AddMethodInfo(const MethodInfo* method )
{
    auto declared = methodsBlock.find(method->GetName());
    if(declared != methodsBlock.end()) {
        throw DeclarationException("Method redeclaration, " +
                                       method->GetReturnType().GetTypeString() + " " +
                                       method->GetName()->GetString() +
                                       " already defined at " + declared->second->GetPosition().ToString(),
                                       method->GetPosition());
    }
    methodsName.push_back(method->GetName());
    methodsBlock.insert(std::make_pair(method->GetName(),
                                         std::unique_ptr<const MethodInfo>(method)));
}

void ClassInfo::AddVariableInfo(const VariableInfo* variable )
{
    auto declared = variableBlock.find(variable->GetName());
    if(declared != variableBlock.end()) {
        throw DeclarationException("Variable redeclaration, " +
                                       declared->second->GetType().GetTypeString() + " " +
                                       variable->GetName()->GetString() +
                                       " already defined at " + declared->second->GetPosition().ToString(),
                                       variable->GetPosition());
    }
    varsName.push_back(variable->GetName());
    variableBlock.insert(std::make_pair(variable->GetName(),
                                          std::unique_ptr<const VariableInfo>(variable)));
}

const VariableBlock& ClassInfo::GetVariableBlock() const
{
    return variableBlock;
}

const MethodBlock& ClassInfo::GetMethodsBlock() const
{
    return methodsBlock;
}

}
