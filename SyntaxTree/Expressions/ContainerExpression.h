#ifndef CONTAINEREXPRESSION_H
#define CONTAINEREXPRESSION_H
#include "common.h"

#include "Expression.h"

struct ContainerExpression : public IExpression
{
    std::unique_ptr<const IExpression> Expression;

    ContainerExpression(Coordinates coords, const IExpression* expression) :
        coords(coords),
        Expression(expression)
    {

    }

    ACCEPT_VISITOR
};

#endif // CONTAINEREXPRESSION_H
