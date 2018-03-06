#include <iostream>
#include "InRegAccess.h"

namespace ActivationRecords {

const char* const ArPrefix = "AR::";

InRegAccess::InRegAccess(T_RecordsType _type, int _size, const std::string& name) :
    type(_type),
    size(_size),
    id(-1),
    name(name),
    temp(new IR::Temp(ArPrefix + name))
{
}

InRegAccess::InRegAccess(T_RecordsType _type, int _size, int id) :
    type(_type),
    size(_size),
    id(id),
    name(std::to_string(id)),
    temp(new IR::Temp(ArPrefix + name))
{
}

InRegAccess::InRegAccess(const InRegAccess& other):
    type(other.type),
    size(other.size),
    id(other.id),
    name(other.name),
    temp(new IR::Temp(*other.temp.get()))
{

}

void InRegAccess::print(TempAddress fp) const
{
    std::cout << "Register: " << name << std::endl;
}

IR::IExp* InRegAccess::GetExp(IR::Temp* fp, const SymbolTable::Position& position) const
{
    delete fp;
    return new IR::Mem(new IR::Temp(*temp.get()));
}

}
