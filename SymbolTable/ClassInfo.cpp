#pragma once

#include <vector>
#include <map>
#include "MethodInfo.h"
#include "VariableInfo.h"
#include "ClassInfo.h"

namespace SymbolTable {
  ClassInfo::ClassInfo( std::string _name, Position _position ) :
    name(_name),
    position(_position)
    {
    }

  void ClassInfo::AddMethodInfo( const std::string name );
  void ClassInfo::AddVariableInfo( const std::string name, const VariableInfo * info );
  const VariableInfo& ClassInfo::GetVariableInfo( const std::string name ) const;
  const MethodInfo& ClassInfo::GetMethodInfo( const std::string name ) const;
  const std::vector<MethodInfo>& ClassInfo::GetMethods() const;
  void ClassInfo::addBlock( std::unordered_map<std::string, Symbol*> * block );
  std::vector<std::string> ClassInfo::GetMethodName();
  std::vector<std::string>  ClassInfo::GetVarsName();
  int ClassInfo::GetMethodCount();
  int ClassInfo::GetVarsCount();
}