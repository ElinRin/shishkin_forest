#ifndef NOTEXPRESSION_H
#define NOTEXPRESSION_H
#include "common.h"

#include "Expression.h"

namespace AST {

struct NotExpression : public IExpression
{
    std::unique_ptr<const IExpression> Expression;

    NotExpression(Coordinates coords, const IExpression* expression) :
        Expression(expression),
        coords(coords)
    {}

    ACCEPT_AST_VISITOR
};

}

#endif // NOTEXPRESSION_H
