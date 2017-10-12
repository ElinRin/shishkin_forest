#ifndef ARRAYLENGTHEXPRESSION_H
#define ARRAYLENGTHEXPRESSION_H
#include "common.h"

#include "Expression.h"

struct ArrayLengthExpression : public IExpression {
    const IExpression* ArrayExpression;

    ArrayLengthExpression(const IExpression* arrayExpression) :
        ArrayExpression(arrayExpression) {}

    ~ArrayLengthExpression() { delete ArrayExpression; }

    ACCEPT_VISITOR
};

#endif // ARRAYLENGTHEXPRESSION_H
