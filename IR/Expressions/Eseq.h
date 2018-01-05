#pragma once

#include <memory>

#include "IExp.h"
#include "IStm.h"

namespace IR {

class Eseq : public IExp {
    std::unique_ptr<const IStm> Statement;
    std::unique_ptr<const IExp> Expression;

    Eseq(const IStm* statement, const IExp* expression):
        Statement(statement),
        Expression(expression)
    {}
};

}
