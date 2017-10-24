#ifndef BINARYEXPRESSION_H
#define BINARYEXPRESSION_H
#include "common.h"

#include "Expression.h"
#include "Visitor.h"

namespace AST {

enum T_BinaryExpressionType {
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
    const T_BinaryExpressionType Type;
    std::unique_ptr<const IExpression> Left;
    std::unique_ptr<const IExpression> Right;

    BinaryExpression(Coordinates coords,
                     const T_BinaryExpressionType type,
                     const IExpression* left,
                     const IExpression* right) :
        coords(coords),
        Type(type),
        Left(left),
        Right(right) {}

    ACCEPT_VISITOR
};

}

#endif // BINARYEXPRESSION_H
