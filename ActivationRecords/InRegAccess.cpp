#pragma once

#include "Symbol.h"
#include "InRegAccess.h"

InRegAccess::InRegAccess(int _size, SymbolTable::Symbol* _symbol) :
  size(_size),
  symbol(_symbol)
{
}


const int InRegAccess::GetSize() {
    return size;
}

const SymbolTable::Symbol* InRegAccess::Symbol() {
    return symbol;
}