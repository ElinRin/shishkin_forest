#ifndef ASSIGNSTATEMENT_H
#define ASSIGNSTATEMENT_H
#include "common.h"

#include "Statement.h"
#include "Expressions/IdExpression.h"

struct AssignStatement : public IStatement
{
    const Id* Identifier;
    const IExpression* ExpressionToAssign;

    AssignStatement(Coordinates coords,  const Id* identifier,
                    const IExpression* expressionToAssign) :
        coords(coords),
        Identifier(identifier),
        ExpressionToAssign(expressionToAssign)
    {}

    ~AssignStatement() {
        delete Identifier;
        delete ExpressionToAssign;
    }

    ACCEPT_VISITOR
};


#endif
