#pragma once

#include "common.h"

#include <string>

#include "Symbol.h"
#include "TempAddress.h"
#include "IExp.h"
#include "Temp.h"

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
    virtual ~IAccess() {}
    virtual const T_RecordsType GetRecordType() = 0;
    virtual const int GetSize() const = 0;
    virtual IR::IExp* GetExp(IR::Temp* fp, const SymbolTable::Position& position) const = 0;
    virtual void print(TempAddress fp) const = 0;
};

}
