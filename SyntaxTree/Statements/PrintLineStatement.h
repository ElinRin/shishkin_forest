#ifndef PRINTLINESTATEMENT_H
#define PRINTLINESTATEMENT_H
#include "common.h"

#include "Statement.h"

namespace AST {

struct PrintLineStatement : public IStatement {
    std::unique_ptr<const IExpression> ExpressionToPrint;

    PrintLineStatement(Coordinates coords,
                       const IExpression* expressionToPrint) :
        coords(coords),
        ExpressionToPrint(expressionToPrint)
    {
    }

    ACCEPT_VISITOR
};

}

#endif
