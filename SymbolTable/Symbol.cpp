#include "Symbol.h"
#include "StringSymbol.h"

namespace SymbolTable { 

Symbol::Symbol(std::string name, const Position &position) :
    name(StringSymbol::GetIntern(name)),
    position(position) {}

Symbol::Symbol(StringSymbol *name, const Position &position) :
    name(name),
    position(position){}

bool Symbol::operator ==( const Symbol& a )
{
    return &a == this;
}

bool Symbol::operator !=( const Symbol& a )
{
    return &a != this;
}

}
