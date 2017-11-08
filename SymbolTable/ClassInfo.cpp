#include <vector>
#include <unordered_map>
#include "MethodInfo.h"
#include "VariableInfo.h"
#include "ClassInfo.h"
#include "Symbol.h"

namespace SymbolTable {

ClassInfo::ClassInfo( std::string _name, Position _position ) :
    Symbol(_name,  _position),
    superClass(nullptr)
{}

void ClassInfo::AddMethodInfo(const MethodInfo* method )
{
      methodsName.push_back(method->GetName());
      methodsBlock.insert(std::make_pair(method->GetName(),
                                         std::unique_ptr<const MethodInfo>(method)));
}

void ClassInfo::AddVariableInfo(const VariableInfo* variable )
{
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
