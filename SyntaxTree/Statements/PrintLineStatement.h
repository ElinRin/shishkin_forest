#ifndef PRINTLINESTATEMENT_H
#define PRINTLINESTATEMENT_H
#include "common.h"

#include "Statement.h"

namespace AST {

struct PrintLineStatement : public IStatement {
    std::unique_ptr<const IExpression> ExpressionToPrint;

    PrintLineStatement(Coordinates coords,
                       const IExpression* expressionToPrint) :
        ExpressionToPrint(expressionToPrint),
        coords(coords)
    {
    }

    ACCEPT_AST_VISITOR
};

}

#endif
