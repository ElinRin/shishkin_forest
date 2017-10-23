#ifndef ASSIGNARRAYELEMENTSEQUENCE_H
#define ASSIGNARRAYELEMENTSEQUENCE_H
#include "common.h"

#include "Statement.h"

struct AssignArrayElementStatement : public IStatement
{
    const Id* Identifier;
    const IExpression* ElementNumberExpression;
    const IExpression* ExpressionToAssign;

    AssignArrayElementStatement(Coordinates& coords,
                       const Id* identifier,
                       const IExpression* elementNumberExpression,
                       const IExpression* expressionToAssign) :
      coords(coords),
      Identifier(identifier),
      ElementNumberExpression(elementNumberExpression),
      ExpressionToAssign(expressionToAssign)
    {}

    ~AssignArrayElementStatement() {
        delete Identifier;
        delete ElementNumberExpression;
        delete ExpressionToAssign;
    }

    ACCEPT_VISITOR
};

#endif
