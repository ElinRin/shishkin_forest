#ifndef ARRAYMEMBEREXPRESSION_H
#define ARRAYMEMBEREXPRESSION_H
#include "common.h"

#include "Expression.h"

struct ArrayMemberExpression : public IExpression
{
    const IExpression* BaseExpression;
    const IExpression* ElementNumberExpression;

    ArrayMemberExpression(const IExpression* baseExpression,
                          const IExpression* elementNumberExpression) :
        BaseExpression(baseExpression),
        ElementNumberExpression(elementNumberExpression) {}

    ~ArrayMemberExpression() {
        delete BaseExpression;
        delete ElementNumberExpression;
    }

    ACCEPT_VISITOR
};

#endif // ARRAYMEMBEREXPRESSION_H
