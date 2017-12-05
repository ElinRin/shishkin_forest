#pragma once

#include "Symbol.h"
#include "InFrameAccess.h"

InFrameAccess::InFrameAccess(T_RecordsType _type, int _size, SymbolTable::Symbol* _symbol) :
    type(_type),
    size(_size),
    symbol(_symbol)
{
}

const T_RecordsType InFrameAccess::GetType() {
    return type;
}

const int InFrameAccess::GetSize() {
    return size;
}

const SymbolTable::Symbol* InFrameAccess::Symbol() {
    return symbol;
}