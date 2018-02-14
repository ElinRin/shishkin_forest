#include <iostream>
#include "InRegAccess.h"
#include "IRNodeTypes.h"

namespace ActivationRecords {

InRegAccess::InRegAccess(T_RecordsType _type, int _size, const std::string& name) :
    type(_type),
    size(_size),
    id(-1),
    name(name)
{
}

InRegAccess::InRegAccess(T_RecordsType _type, int _size, int id) :
    type(_type),
    size(_size),
    id(id),
    name(std::to_string(id))
{
}

void InRegAccess::print(TempAddress fp) const
{
    std::cout << "Register: " << name << std::endl;
}

const IR::IExp* InRegAccess::GetExp(const IR::Temp* fp, const SymbolTable::Position& position) const
{
    delete fp;
    return new IR::Temp(name, position);
}

}
