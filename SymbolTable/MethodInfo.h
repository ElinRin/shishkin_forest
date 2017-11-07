#pragma once

#include "VariableInfo.h"
#include "Position.h"
#include "Symbol.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace SymbolTable {

class MethodInfo : public Symbol{
public:
    MethodInfo( std::string _name, Position _position, TypeInfo _returnType);
    TypeInfo GetReturnType() const;
    void AddVariableInfo(const VariableInfo *name );
    const VariableInfo* GetVariableInfo(const StringSymbol *name , const Position &position) const;
    void AddArgInfo(const VariableInfo *name );
    std::vector<const StringSymbol*> GetArgsName() const;
    std::vector<const StringSymbol*>  GetVarsName() const;
    int GetArgsCount();
    int GetVarsCount();
    const VariableBlock& GetVariableBlocks() const { return block; }

private:
    TypeInfo returnType;
    std::vector<const StringSymbol*> varsName;
    std::vector<const StringSymbol*> argsName;
    VariableBlock block;
};

typedef std::unordered_map< const StringSymbol*, std::unique_ptr<const MethodInfo>>  MethodBlock;

}
