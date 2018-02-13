#pragma once

#include <vector>
#include <map>

#include "MethodInfo.h"

namespace SymbolTable {

interface IClassStruct {
    virtual ~IClassStruct() {}

    virtual void AddToVtable(const MethodInfo* info) = 0;
    virtual void AddField(const VariableInfo* info) = 0;

    virtual const IR::IExp* GetFieldFrom(const StringSymbol* fieldName, const IR::IExp* base, const Position& position) const = 0;
    virtual const IR::IExp* GetVirtualMethodAddress(const StringSymbol* methodName,
                                            const IR::IExp* base, const Position& position) const = 0;
    virtual const IR::IExp* AllocateNew(const Position& position) const = 0;
};

}
