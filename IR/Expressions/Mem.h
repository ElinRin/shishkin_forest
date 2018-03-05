#pragma once

#include "IExp.h"
#include <memory>

namespace IR {

class Mem: public IExp {
public:
    std::unique_ptr<IExp> Expression;

    Mem(IExp* expression, const Coords& coords=Coords()) :
        Expression(expression),
        coords(coords)
    {}

    virtual bool IsCommutative() const override { return false; }

    ACCEPT_IR_VISITOR
};

}
