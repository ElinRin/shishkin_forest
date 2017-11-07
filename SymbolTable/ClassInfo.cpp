#pragma once

#include <vector>
#include <unordered_map>
#include "MethodInfo.h"
#include "VariableInfo.h"
#include "ClassInfo.h"
#include "Symbol.h"

namespace SymbolTable {
  ClassInfo::ClassInfo( std::string _name, Position _position ) :
    name(_name),
    position(_position)
    {
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
  
  const VariableInfo& ClassInfo::GetVariableInfo( const std::string name ) const
  {
		return block.find(name)->second;
  }
  
  const MethodInfo& ClassInfo::GetMethodInfo( const std::string name ) const
  {
		return block.find(name)->second;
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