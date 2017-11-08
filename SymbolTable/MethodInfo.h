#pragma once

#include "VariableInfo.h"
#include "Position.h"
#include "StringSymbol.h"
#include "Symbol.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace SymbolTable {

enum T_Qualifier {
    Q_Public,
    Q_Private
};

class MethodInfo : public Symbol{
public:
    MethodInfo(std::string _name, Position _position, TypeInfo _returnType,
                T_Qualifier _qualifier);
    TypeInfo GetReturnType() const;
    void AddVariableInfo(const VariableInfo *name );
    const VariableInfo* GetVariableInfo(const StringSymbol *name , const Position &position) const;
    void AddArgInfo(const VariableInfo *name );
    const std::vector<const StringSymbol*>& GetArgsName() const;
    const std::vector<const StringSymbol*>& GetVarsName() const;
    int GetArgsCount();
    int GetVarsCount();
    const VariableBlock& GetVariableBlocks() const { return block; }
    const T_Qualifier GetQualifier() const { return qualifier; }

private:
    TypeInfo returnType;
    T_Qualifier qualifier;
    std::vector<const StringSymbol*> varsName;
    std::vector<const StringSymbol*> argsName;
    VariableBlock block;
};

typedef std::unordered_map< const StringSymbol*, std::unique_ptr<const MethodInfo>>  MethodBlock;

}
