#ifndef IFELSESTATEMENT_H
#define IFELSESTATEMENT_H
#include "common.h"

#include "Statement.h"

struct IfElseStatement : public IStatement {
    const IExpression* Condition;
    const IStatement* IfStatement;
    const IStatement* ElseStatement;

    IfElseStatement(Coordinates coords, const IExpression* condition,
                    const IStatement* ifStatement,
                    const IStatement* elseStatement) :
        coords(coords),
        Condition(condition),
        IfStatement(ifStatement),
        ElseStatement(elseStatement)
    {
    }

    ~IfElseStatement() {
        delete Condition;
        delete IfStatement;
        delete ElseStatement;
    }

    ACCEPT_VISITOR
};


#endif
