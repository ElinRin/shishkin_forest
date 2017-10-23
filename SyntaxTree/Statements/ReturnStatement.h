#ifndef RETURNSTATEMENT_H
#define RETURNSTATEMENT_H
#include "common.h"

#include "Expression.h"

struct ReturnStatement : public ITreeNode
{
    const IExpression* Expression;

    ReturnStatement(Coordinates& coords,
                    const IExpression* expression) :
        coords(coords),
        Expression(expression)
    {}

    ACCEPT_VISITOR
};

#endif
