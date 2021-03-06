#ifndef RETURNSTATEMENT_H
#define RETURNSTATEMENT_H
#include "common.h"

#include "Expressions/Expression.h"

namespace AST {

struct ReturnStatement : public ITreeNode
{
    std::unique_ptr<const IExpression> Expression;

    ReturnStatement(Coordinates coords,
                    const IExpression* expression) :
        Expression(expression),
        coords(coords)
    {}

    ACCEPT_AST_VISITOR
};

}

#endif
