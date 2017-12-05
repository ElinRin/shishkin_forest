#pragma once

#include "common.h"
#include "Symbol.h"

namespace ActivationRecords {

interface IFrame {
public:
    void AddFormal( const SymbolTable::Symbol* name);
    void AddLocal( const SymbolTable::Symbol* name);
    int FormalsCount() const;
    const IAccess* Formal( int index ) const;
    const IAccess* FindLocalOrFormal( const SymbolTable::Symbol* name ) const;

};

}