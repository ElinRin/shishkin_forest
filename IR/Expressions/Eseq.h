#pragma once

#include <memory>

#include "IExp.h"
#include "IStm.h"

namespace IR {

class Eseq : public IExp {
public:
    std::unique_ptr<IStm> Statement;
    std::unique_ptr<IExp> Expression;

    Eseq(IStm* statement, IExp* expression, const Coords& coords=Coords()):
        Statement(statement),
        Expression(expression),
        coords(coords)
    {}

    virtual bool IsCommutative() const override { return false; }

    ACCEPT_IR_VISITOR
};

}
