#pragma once

#include <string>

namespace ActivationRecords {

class Temp {
public:
    Temp(int offset = 0);
    Temp(int offset, int address);
    Temp(Temp& t):offset(t.offset), address(t.address) {}

    int GetAddress() const { return address + offset; }
    Temp AtAddress(int _address) const { return Temp(offset, _address + address); }
    std::string GetAddressString() const;
    Temp operator=(const Temp& t);
private:
    const int offset;
    int address;
};

}
