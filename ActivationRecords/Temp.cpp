#include <string>
#include "Temp.h"

namespace ActivationRecords {
  
Temp::Temp(int _x, int _y) :
  x(_x),
  y(_y)
{
}

std::string Temp::ToString() const
{
    return "(" + std::to_string(y) + "," + std::to_string(x) + ")";
}

}
