#pragma once

#include "IExp.h"
#include <memory>

namespace IR {

class Temp : public IExp {
public:
    std::unique_ptr<const AR::Temp> Temp;

    Temp(AR::Temp* temp) : Temp(temp) { }
};

}
