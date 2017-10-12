#ifndef CONTAINEREXPRESSION_H
#define CONTAINEREXPRESSION_H
#include "common.h"

#include "Expression.h"

struct ContainerExpression : public IExpression
{
    const IExpression* Expression;

    ContainerExpression(const IExpression* expression) :
    Expression(expression) {}

    ACCEPT_VISITOR
};

#endif // CONTAINEREXPRESSION_H
