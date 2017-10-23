#ifndef NEWINTARRAYEXPRESSION_H
#define NEWINTARRAYEXPRESSION_H
#include "common.h"

#include "Expression.h"

struct NewIntArrayExpression : public IExpression
{
    const IExpression* NumberOfElements;

    NewIntArrayExpression(Coordinates coords, const IExpression* numberOfElements):
        coords(coords),
        NumberOfElements(numberOfElements){}
    ~NewIntArrayExpression() { delete NumberOfElements; }

    ACCEPT_VISITOR
};
#endif // NEWINTARRAYEXPRESSION_H
