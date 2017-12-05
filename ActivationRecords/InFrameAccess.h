#pragma once

#include "Access.h"
#include "Symbol.h"

namespace ActivationRecords {

class CInFrameAccess : public IAccess {
public: 
    InFrameAccess(int _size, SymbolTable::Symbol* _symbol);
};

}