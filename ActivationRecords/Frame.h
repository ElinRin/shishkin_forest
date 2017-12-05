#pragma once

#include "common.h"
#include "Access.h"
#include "Temp.h"

namespace ActivationRecords {

interface IFrame {
public:
    void AddFormal( const SymbolTable::Symbol* name) = 0;
    void AddLocal( const SymbolTable::Symbol* name) = 0;
    int FormalsCount() const = 0;
    const IAccess* Formal( int index ) const = 0;
    const IAccess* FindLocalOrFormal( const SymbolTable::Symbol* name ) const = 0;
    const Temp* FP() const = 0;
    int WordSize() const = 0;
};

}
