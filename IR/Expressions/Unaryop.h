#pragma once

#include "IExp.h"

namespace IR {

class Unaryop : public IExp {
public:
    enum TUnaryop {
        TU_NOT
    };

    const TUnaryop Operation;
    std::unique_ptr<IExp> Expression;

    Unaryop(TUnaryop operation, IExp* expression, const Coords& coords=Coords()) :
        Operation(operation),
        Expression(expression),
        coords(coords)
    {
    }

    virtual bool IsCommutative() const override { return false; }
    virtual bool IsAbsolutelyCommutative() const override { return false; }

    ACCEPT_IR_VISITOR
};

}
