#pragma once

#include "common.h"
#include "Access.h"
#include "VariableInfo.h"
#include "Temp.h"

namespace ActivationRecords {

interface IFrame {
public:
    virtual void AddFormal( const SymbolTable::VariableInfo& name) = 0;
    virtual void AddLocal( const SymbolTable::VariableInfo& name) = 0;
    virtual int FormalsCount() const = 0;
    virtual const IAccess* Formal( int index ) const = 0;
    virtual const IAccess* FindLocalOrFormal( const StringSymbol* name ) const = 0;
    virtual const Temp FP() const = 0;
    virtual const Temp SP() const = 0;
    virtual int WordSize() const = 0;
};

}
