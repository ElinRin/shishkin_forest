#pragma once

#include "IStm.h"
#include "IExp.h"

#include <memory>

namespace IR {

class Exp : public IStm {
public:
    std::unique_ptr<const IExp> expression;

    Exp(const IExp* expression) : expression(expression) {  }
};

}
