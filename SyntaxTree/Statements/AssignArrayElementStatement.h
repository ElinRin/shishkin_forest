#ifndef ASSIGNARRAYELEMENTSEQUENCE_H
#define ASSIGNARRAYELEMENTSEQUENCE_H
#include "common.h"

#include "Statement.h"

namespace AST {

struct AssignArrayElementStatement : public IStatement
{
    std::unique_ptr<const Id> Identifier;
    std::unique_ptr<const IExpression> ElementNumberExpression;
    std::unique_ptr<const IExpression> ExpressionToAssign;

    AssignArrayElementStatement(Coordinates coords,
                       const Id* identifier,
                       const IExpression* elementNumberExpression,
                       const IExpression* expressionToAssign) :
      coords(coords),
      Identifier(identifier),
      ElementNumberExpression(elementNumberExpression),
      ExpressionToAssign(expressionToAssign)
    {}

    ACCEPT_VISITOR
};

}

#endif
