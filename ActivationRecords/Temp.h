#pragma once

#include <string>

namespace ActivationRecords {

class Temp {
public:
    Temp(int _x, int _y);
    std::string GetAddress() const;
private:
    int x;
    int y;
};

}
