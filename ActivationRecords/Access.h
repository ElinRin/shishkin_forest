#pragma once

#include "common.h"
#include "Symbol.h"
#include "Temp.h"
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
    virtual ~IAccess() {}
    virtual const T_RecordsType GetRecordType() = 0;
    virtual const int GetSize() const = 0;
    virtual void print(Temp fp) const = 0;
};

}
