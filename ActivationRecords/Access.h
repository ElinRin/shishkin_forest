#pragma once

#include "common.h"
#include "Symbol.h"

namespace ActivationRecords {

interface IAccess {
public:
    virtual ~IAccess() {}
    const int GetSize();
    const SymbolTable::Symbol* Symbol();

protected:
    const int size;
    const SymbolTable::Symbol* symbol;
};

}