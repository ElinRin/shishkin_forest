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
    MethodInfo(const std::string& _name, const std::string& className, const Position _position, TypeInfo _returnType,
                T_Qualifier _qualifier);
    const TypeInfo &GetReturnType() const;
    void AddVariableInfo(const VariableInfo *name );
    const VariableInfo* GetVariableInfo(const StringSymbol *name , const Position &position) const;
    void AddArgInfo(const VariableInfo *name );
    const std::vector<const VariableInfo*>& GetArgsNames() const;
    const std::vector<const VariableInfo*>& GetVarsName() const;
    int GetArgsCount() const;
    int GetVarsCount() const;
    const VariableBlock& GetVariableBlocks() const { return block; }
    const T_Qualifier GetQualifier() const { return qualifier; }
    const StringSymbol* GetFullName() const { return className; }

private:
    const StringSymbol* className;
    const TypeInfo returnType;
    const T_Qualifier qualifier;
    std::vector<const VariableInfo*> varsNames;
    std::vector<const VariableInfo*> argsNames;
    VariableBlock block;
};

typedef std::unordered_map< const StringSymbol*, std::unique_ptr<const MethodInfo>>  MethodBlock;

}
