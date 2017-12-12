#pragma once

#include <iostream>

#include "Symbol.h"
#include "InFrameAccess.h"

namespace ActivationRecords {

InFrameAccess::InFrameAccess(T_RecordsType _type, int _size, int offset) :
    recordType(_type),
    size(_size),
    address(Temp(offset))
{
}

const Temp& InFrameAccess::Offset() const
{
     return address;
}

void InFrameAccess::print(Temp fp) const
{
    std::cout << "In frame position " << address.AtAddress(fp.GetAddress()).GetAddress() << std::endl;
}

}
