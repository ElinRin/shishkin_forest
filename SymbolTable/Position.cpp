#include <string>
#include "Position.h"

namespace SymbolTable {

Position::Position() : x(0), y(0)
{

}

Position::Position(int _x, int _y) :
  x(_x),
  y(_y)
{
}

std::string Position::ToString() const
{
    return "(" + std::to_string(y) + "," + std::to_string(x) + ")";
}

}
