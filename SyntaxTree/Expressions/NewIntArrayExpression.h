#ifndef NEWINTARRAYEXPRESSION_H
#define NEWINTARRAYEXPRESSION_H
#include "common.h"

#include "Expression.h"

namespace AST {

struct NewIntArrayExpression : public IExpression
{
    std::unique_ptr<const IExpression> NumberOfElements;

    NewIntArrayExpression(Coordinates coords, const IExpression* numberOfElements):
        coords(coords),
        NumberOfElements(numberOfElements){}

    ACCEPT_VISITOR
};

}

#endif // NEWINTARRAYEXPRESSION_H
