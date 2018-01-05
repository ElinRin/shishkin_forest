#ifndef NEWINTARRAYEXPRESSION_H
#define NEWINTARRAYEXPRESSION_H
#include "common.h"

#include "Expression.h"

namespace AST {

struct NewIntArrayExpression : public IExpression
{
    std::unique_ptr<const IExpression> NumberOfElements;

    NewIntArrayExpression(Coordinates coords, const IExpression* numberOfElements):
        NumberOfElements(numberOfElements),
        coords(coords)
    {}

    ACCEPT_VISITOR
};

}

#endif // NEWINTARRAYEXPRESSION_H
