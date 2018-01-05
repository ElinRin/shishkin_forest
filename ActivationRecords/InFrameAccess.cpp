#include <iostream>

#include "Symbol.h"
#include "InFrameAccess.h"
#include "IRNodeTypes.h"

namespace ActivationRecords {

InFrameAccess::InFrameAccess(T_RecordsType _type, int _size, int offset) :
    size(_size),
    address(TempAddress(offset)),
    recordType(_type)
{
}

const IR::IExp* InFrameAccess::GetExp(const IR::Temp* fp) const
{
    return new IR::Mem(new IR::Binop(IR::Binop::TB_PLUS, fp,
                                     new IR::Const(address.GetAddress())));
}

const TempAddress& InFrameAccess::Offset() const
{
     return address;
}

void InFrameAccess::print(TempAddress fp) const
{
    std::cout << "In frame position " << address.AtAddress(fp.GetAddress()).GetAddress() << std::endl;
}

}
