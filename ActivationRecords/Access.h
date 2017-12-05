#pragma once

#include "common.h"
#include "Symbol.h"

namespace ActivationRecords {

interface IAccess {
public:
    virtual ~IAccess() {}
    const int GetSize() = 0;
    const SymbolTable::Symbol* Symbol() = 0;

protected:
    const int size;
    const SymbolTable::Symbol* symbol;
};

}
