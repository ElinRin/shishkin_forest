#include <string>
#include "TempAddress.h"

namespace ActivationRecords {

TempAddress::TempAddress(int offset) : offset(offset), address(0)
{

}

TempAddress::TempAddress(int offset, int address) :
  offset(offset),
  address(address)
{
}

std::string TempAddress::GetAddressString() const
{
    if(address == 0) {
        return "offset " + std::to_string(offset);
    }
    return "address: " + std::to_string(GetAddress()) + ", " +
            "offset: " + std::to_string(offset);
}

TempAddress TempAddress::operator=(const TempAddress& t) {
 return TempAddress(t.offset, t.address);
}

}
