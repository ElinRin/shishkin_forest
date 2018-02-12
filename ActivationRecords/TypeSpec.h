#pragma once

#include "common.h"
#include "VariableInfo.h"

namespace ActivationRecords {

interface ITypeSpec {
public:
    virtual ~ITypeSpec() {}

    virtual int TypeSize(SymbolTable::T_VariableType type) const = 0;
    virtual int ReferenceSize() const = 0;
    virtual int WordSize() const = 0;
};

}
