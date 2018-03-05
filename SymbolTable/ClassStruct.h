#pragma once

#include <vector>
#include <map>

#include "MethodInfo.h"

namespace SymbolTable {

interface IClassStruct {
    virtual ~IClassStruct() {}

    virtual void AddClassName(const StringSymbol* className) = 0;
    virtual void AddToVtable(const MethodInfo* info) = 0;
    virtual void AddField(const VariableInfo* info) = 0;

    virtual const std::vector<const MethodInfo*>& GetVTable() const = 0;
    virtual const StringSymbol* GetTableName() const = 0;

    virtual IR::IExp* GetFieldFrom(const StringSymbol* fieldName, IR::IExp* base, const Position& position) const = 0;
    virtual IR::IExp* GetVirtualMethodAddress(const StringSymbol* methodName,
                                            IR::IExp* base, const Position& position) const = 0;
    virtual IR::IExp* AllocateNew(const Position& position) const = 0;
};

}
