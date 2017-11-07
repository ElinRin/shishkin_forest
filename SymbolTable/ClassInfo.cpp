#pragma once

#include <vector>
#include <unordered_map>
#include <DeclarationException.h>
#include "MethodInfo.h"
#include "VariableInfo.h"
#include "ClassInfo.h"
#include "Symbol.h"

namespace SymbolTable {
  ClassInfo::ClassInfo( std::string _name, Position _position ) :
    position(_position)
    {
      this->name = _name;
    }

  void MethodInfo::addBlock( std::unordered_map<std::string, Symbol*> * _block )
  {
    block = _block;
  }

  void ClassInfo::AddMethodInfo( const std::string name )
  {
		methodsName.push_back(name);
  }
  
  void ClassInfo::AddVariableInfo( const std::string name )
  {
		varsName.push_back(name);
  }
  
  const VariableInfo* ClassInfo::GetVariableInfo( const std::string name ) const
  {
        const VariableInfo* variable = dynamic_cast<VariableInfo*>(block->find(name)->second);
        if(variable == nullptr) {
            throw new DeclarationException("Variable " + name + " in class " + this->name + "undeclared");
        }
        return variable;
  }
  
  const MethodInfo* ClassInfo::GetMethodInfo( const std::string name ) const
  {
      const MethodInfo* method = dynamic_cast<MethodInfo*>(block->find(name)->second);
      if(method == nullptr) {
          throw new DeclarationException("Method " + name + " in class " + this->name + "undeclared");
      }
      return method;
  }
  
  std::vector<std::string> ClassInfo::GetMethodName()
  {
		return methodsName;
  }
  
  std::vector<std::string>  ClassInfo::GetVarsName()
  {
		return varsName;
  }
  
  int ClassInfo::GetMethodCount() 
  {
		return methodsName.size();
  }
  
  int ClassInfo::GetVarsCount()
  {
    {
      return varsName.size();
    }
  }
}
