#ifndef BINARYEXPRESSION_H
#define BINARYEXPRESSION_H
#include "common.h"

#include "Expression.h"
#include "Visitor.h"

enum BinaryExpressionType {
    BET_And,
    BET_Less,
    BET_Plus,
    BET_Minus,
    BET_Mult,
    BET_Mod,
    BET_Or
};

struct BinaryExpression : public IExpression
{
    const BinaryExpressionType Type;
    const IExpression* Left;
    const IExpression* Right;

    BinaryExpression(Coordinates coords,
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
