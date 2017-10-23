#ifndef BINARYEXPRESSION_H
#define BINARYEXPRESSION_H
#include "common.h"

#include "Expression.h"
#include "Visitor.h"

enum BinaryExpressionType {
    BET_AND,
    BET_LESS,
    BET_PLUS,
    BET_MINUS,
    BET_MULT,
    BET_MOD,
    BET_OR
};

struct BinaryExpression : public IExpression
{
    const BinaryExpressionType Type;
    const IExpression* Left;
    const IExpression* Right;

    BinaryExpression(Coordinates& coords,
                     const BinaryExpressionType type,
                     const IExpression* left,
                     const IExpression* right) :
        coords(coords),
        Type(type),
        Left(left),
        Right(right) {}

    ~BinaryExpression() {
        delete Left;
        delete Right;
    }

    ACCEPT_VISITOR
};
#endif // BINARYEXPRESSION_H
