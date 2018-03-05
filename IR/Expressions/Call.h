#pragma once

#include "List.h"

namespace IR {

class Call : public IExp {
public:
    std::unique_ptr<IExp> FuncExpression;
    std::unique_ptr<ExpList> Arguments;

    Call(IExp* func, ExpList* args, const Coords& coords=Coords()):
        FuncExpression(func),
        Arguments(args),
        coords(coords)
    {}

    virtual bool IsCommutative() const override { return false; }
    virtual bool IsAbsolutelyCommutative() const override { return false; }

    ACCEPT_IR_VISITOR
};

}
