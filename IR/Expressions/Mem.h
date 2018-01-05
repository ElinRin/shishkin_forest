#pragma once

#include "IExp.h"
#include <memory>

namespace IR {

class Mem: public IExp {
public:
    std::unique_ptr<const IExp> Expression;

    Mem(const IExp* expression):
        Expression(expression)
    { }
};

}
