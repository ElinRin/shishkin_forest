#ifndef CONTAINEREXPRESSION_H
#define CONTAINEREXPRESSION_H
#include "common.h"

#include "Expression.h"

namespace AST {

struct ContainerExpression : public IExpression
{
    std::unique_ptr<const IExpression> Expression;

    ContainerExpression(Coordinates coords, const IExpression* expression) :
        Expression(expression),
        coords(coords)
    {

    }

    ACCEPT_VISITOR
};

}

#endif // CONTAINEREXPRESSION_H
