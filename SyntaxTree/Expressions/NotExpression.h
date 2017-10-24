#ifndef NOTEXPRESSION_H
#define NOTEXPRESSION_H
#include "common.h"

#include "Expression.h"

namespace AST {

struct NotExpression : public IExpression
{
    std::unique_ptr<const IExpression> Expression;

    NotExpression(Coordinates coords, const IExpression* expression) :
        coords(coords),
        Expression(expression)  {}

    ACCEPT_VISITOR
};

}

#endif // NOTEXPRESSION_H
