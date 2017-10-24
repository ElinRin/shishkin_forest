#ifndef IFELSESTATEMENT_H
#define IFELSESTATEMENT_H
#include "common.h"

#include "Statement.h"

namespace AST {

struct IfElseStatement : public IStatement {
    std::unique_ptr<const IExpression> Condition;
    std::unique_ptr<const IStatement> IfStatement;
    std::unique_ptr<const IStatement> ElseStatement;

    IfElseStatement(Coordinates coords, const IExpression* condition,
                    const IStatement* ifStatement,
                    const IStatement* elseStatement) :
        coords(coords),
        Condition(condition),
        IfStatement(ifStatement),
        ElseStatement(elseStatement)
    {
    }

    ACCEPT_VISITOR
};

}

#endif
