#pragma once

#include "IExp.h"

namespace IR {

class Unaryop : public IExp {
public:
    enum TUnaryop {
        TU_NOT
    };

    const TUnaryop Operation;
    std::unique_ptr<const IExp> Expression;

    Unaryop(TUnaryop operation, const IExp* expression, const Coords& coords=Coords()) :
        Operation(operation),
        Expression(expression),
        coords(coords)
    {
    }

    ACCEPT_IR_VISITOR
};

}
