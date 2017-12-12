#include <string>
#include "Temp.h"

namespace ActivationRecords {

Temp::Temp(int offset) : offset(offset), address(0)
{

}

Temp::Temp(int offset, int address) :
  offset(offset),
  address(address)
{
}

std::string Temp::GetAddressString() const
{
    if(address == 0) {
        return "offset " + std::to_string(offset);
    }
    return "address: " + std::to_string(GetAddress()) + ", " +
            "offset: " + std::to_string(offset);
}

Temp Temp::operator=(const Temp& t) {
 return Temp(t.offset, t.address);
}

}
