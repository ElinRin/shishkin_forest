#pragma once

#include "MethodInfo.h"
#include "VariableInfo.h"
#include "Position.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace SymbolTable {

	MethodInfo( std::string _name, Position _position, std::string _returnType) : 
		name(_name),
		position(_position),
		returnType(_returnType)
		{	
		}

	int MethodInfo::GetReturnType() const
	{
		return returnType;
	}

	void MethodInfo::AddVariableInfo( const std::string name )
	{
		varsName.push_back(name);
	}

	const VariableInfo & MethodInfo::GetVariableInfo( const std::string name ) const
	{
		return block.find(name)->second;
	}

	void MethodInfo::AddArgInfo( const std::string name )
	{
		argsName.push_back(name);
	}

	const VariableInfo & MethodInfo::GetArgInfo( const std::string name  ) const
	{
		return block.find(name)->second;
	}

	void MethodInfo::addBlock( std::unordered_map<std::string, Symbol*> * _block )
	{
		block = _block;
	}
	
	std::vector<std::string> MethodInfo::GetArgsName()
	{
		return argsName;
	}

	std::vector<std::string>  MethodInfo::GetVarsName()
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