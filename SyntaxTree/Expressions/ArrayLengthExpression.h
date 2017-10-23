#ifndef ARRAYLENGTHEXPRESSION_H
#define ARRAYLENGTHEXPRESSION_H
#include "common.h"

#include "Expression.h"

struct ArrayLengthExpression : public IExpression {
    const IExpression* ArrayExpression;

    ArrayLengthExpression(Coordinates coords,
                          const IExpression* arrayExpression) :
        coords(coords),
        ArrayExpression(arrayExpression) {}

    ~ArrayLengthExpression() { delete ArrayExpression; }

    ACCEPT_VISITOR
};

#endif // ARRAYLENGTHEXPRESSION_H
