#pragma once

#include "IExp.h"
#include <memory>

namespace IR {

class Mem: public IExp {
public:
    std::unique_ptr<const IExp> Expression;

    Mem(const IExp* expression, const Coords& coords=Coords()) :
        Expression(expression),
        coords(coords)
    {}

    ACCEPT_IR_VISITOR
};

}
