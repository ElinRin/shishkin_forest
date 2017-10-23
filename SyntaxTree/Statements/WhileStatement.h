#ifndef WHILESTATEMENT_H
#define WHILESTATEMENT_H
#include "common.h"

#include "Statement.h"

struct WhileStatement : public IStatement
{
    const IExpression* Condition;
    const IStatement* Body;

    WhileStatement(const Coordinates& coords,
                   const IExpression* condition,
                   const IStatement* statement) :
        coords(coords),
        Condition(condition),
        Body(statement)
    {}

    ~WhileStatement() {
        delete Condition;
        delete Body;
    }

    ACCEPT_VISITOR
};

#endif
