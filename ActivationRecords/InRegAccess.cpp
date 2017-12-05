#pragma once

#include "Symbol.h"
#include "InRegAccess.h"

InRegAccess::InRegAccess(T_RecordsType _type, int _size, SymbolTable::Symbol* _symbol) :
    type(_type),
    size(_size),
    symbol(_symbol)
{
}

const T_RecordsType InRegAccess::GetType() {
    return type;
}

const int InRegAccess::GetSize() {
    return size;
}

const SymbolTable::Symbol* InRegAccess::Symbol() {
    return symbol;
}