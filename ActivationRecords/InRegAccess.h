#pragma once

#include "Access.h"
#include "Symbol.h"

namespace ActivationRecords {

class CInRegAccess : public IAccess {
public: 
    InRegAccess(int _size, SymbolTable::Symbol* _symbol);
};

}