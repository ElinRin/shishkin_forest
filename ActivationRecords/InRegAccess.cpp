#pragma once

#include <iostream>
#include "InRegAccess.h"

namespace ActivationRecords {

InRegAccess::InRegAccess(T_RecordsType _type, int _size, int regNumber) :
    type(_type),
    size(_size),
    regNumber(regNumber)
{
}

void InRegAccess::print(Temp fp) const
{
    std::cout << "Register: " << regNumber << std::endl;
}

}
