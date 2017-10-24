#ifndef ASSIGNSTATEMENT_H
#define ASSIGNSTATEMENT_H
#include "common.h"

#include "Statement.h"
#include "Expressions/IdExpression.h"

namespace AST {

struct AssignStatement : public IStatement
{
    std::unique_ptr<const Id> Identifier;
    std::unique_ptr<const IExpression> ExpressionToAssign;

    AssignStatement(Coordinates coords,  const Id* identifier,
                    const IExpression* expressionToAssign) :
        coords(coords),
        Identifier(identifier),
        ExpressionToAssign(expressionToAssign)
    {}

    ACCEPT_VISITOR
};

}

#endif
