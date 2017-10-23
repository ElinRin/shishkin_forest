#ifndef NOTEXPRESSION_H
#define NOTEXPRESSION_H
#include "common.h"

#include "Expression.h"

struct NotExpression : public IExpression
{
    const IExpression* Expression;

    NotExpression(Coordinates coords, const IExpression* expression) :
        coords(coords),
        Expression(expression)  {}
    ~NotExpression() { delete Expression; }

    ACCEPT_VISITOR
};

#endif // NOTEXPRESSION_H
