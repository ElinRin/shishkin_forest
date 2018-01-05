#pragma once

#include <string>

namespace ActivationRecords {

class TempAddress {
public:
    TempAddress(int offset = 0);
    TempAddress(int offset, int address);
    TempAddress(TempAddress& t):offset(t.offset), address(t.address) {}

    int GetAddress() const { return address + offset; }
    TempAddress AtAddress(int _address) const { return TempAddress(offset, _address + address); }
    std::string GetAddressString() const;
    TempAddress operator=(const TempAddress& t);
private:
    const int offset;
    int address;
};

}
