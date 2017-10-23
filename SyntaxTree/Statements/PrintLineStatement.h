#ifndef PRINTLINESTATEMENT_H
#define PRINTLINESTATEMENT_H
#include "common.h"

#include "Statement.h"

struct PrintLineStatement : public IStatement {
    const IExpression* ExpressionToPrint;

    PrintLineStatement(Coorsinates& coords,
                       const IExpression* expressionToPrint) :
        coords(coords),
        ExpressionToPrint(expressionToPrint)
    {
    }

    ~PrintLineStatement() {
        delete ExpressionToPrint;
    }

    ACCEPT_VISITOR
};

#endif
