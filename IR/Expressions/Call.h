#pragma once

#include "List.h"

namespace IR {

class Call : public IExp {
public:
    std::unique_ptr<const IExp> FuncExpression;
    std::unique_ptr<const ExpList> Arguments;

    Call(const IExp* func, const ExpList* args):
        FuncExpression(func),
        Arguments(args)
    {}
};

}
