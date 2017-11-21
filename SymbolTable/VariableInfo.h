#pragma once

#include <unordered_map>

#include "Symbol.h"
#include "Position.h"

namespace SymbolTable {

enum T_VariableType {
    VT_Int,
    VT_IntArray,
    VT_Boolean,
    VT_UserClass
};

class TypeInfo {
public:
    TypeInfo(T_VariableType type, const StringSymbol* userClass = nullptr);
    TypeInfo(T_VariableType type, const std::string userClass);

    T_VariableType GetType() const { return type; }
    const std::string GetTypeString() const;
    const StringSymbol* GetUserClass() const;
    bool operator==(const TypeInfo& other) const;

private:
    T_VariableType type;
    const StringSymbol* userClass;
};

class VariableInfo : public Symbol {
public:
    const TypeInfo& GetType() const;
    VariableInfo(std::string _name, const Position _position, TypeInfo _type );
private:
    TypeInfo type;
};

typedef std::unordered_map< const StringSymbol*, std::unique_ptr<const VariableInfo>>  VariableBlock;

}
