#pragma once

#include <memory>

#include "IExp.h"
#include "IStm.h"

namespace IR {

class Eseq : public IExp {
public:
    std::unique_ptr<const IStm> Statement;
    std::unique_ptr<const IExp> Expression;

    Eseq(const IStm* statement, const IExp* expression, const Coords& coords=Coords()):
        Statement(statement),
        Expression(expression),
        coords(coords)
    {}
    ACCEPT_IR_VISITOR
};

}
