#pragma once

#include "common.h"
#include "Symbol.h"
#include <string>

namespace ActivationRecords {

enum T_RecordsType {
    RT_Formal,
    RT_Local,
    RT_FramePointer,
    RT_StackPointer,
    RT_AddressExit,
    RT_AddressReturnValue
};


interface IAccess {
public:
    IAccess(T_RecordsType _type, int _size, SymbolTable::Symbol* _symbol);
    virtual ~IAccess() {};
    const int GetSize();
    const T_RecordsType GetType();
    const SymbolTable::Symbol* Symbol();

protected:
    const T_RecordsType type;
    const int size;
    const SymbolTable::Symbol* symbol;
};

}