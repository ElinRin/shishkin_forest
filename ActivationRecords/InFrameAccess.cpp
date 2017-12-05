#pragma once

#include "Symbol.h"
#include "InFrameAccess.h"

InFrameAccess::InFrameAccess(int _size, SymbolTable::Symbol* _symbol) :
  size(_size),
  symbol(_symbol)
{
}

const int InFrameAccess::GetSize() {
    return size;
}

const SymbolTable::Symbol* InFrameAccess::Symbol() {
    return symbol;
}