#include <iostream>
#include "InRegAccess.h"
#include "IRNodeTypes.h"

namespace ActivationRecords {

InRegAccess::InRegAccess(T_RecordsType _type, int _size, int regNumber) :
    type(_type),
    size(_size),
    regNumber(regNumber)
{
}

void InRegAccess::print(TempAddress fp) const
{
    std::cout << "Register: " << regNumber << std::endl;
}

const IR::IExp* InRegAccess::GetExp(const IR::Temp* fp) const
{
    delete fp;
    return new IR::Temp(regNumber);
}

}
