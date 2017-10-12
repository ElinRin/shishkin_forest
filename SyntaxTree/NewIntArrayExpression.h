#ifndef NEWINTARRAYEXPRESSION_H
#define NEWINTARRAYEXPRESSION_H
#include "common.h"

#include "Expression.h"

struct NewIntArrayExpression : public IExpression
{
    const IExpression* NumberOfElements;

    NewIntArrayExpression(const IExpression* numberOfElements):
        NumberOfElements(numberOfElements){}
    ~NewIntArrayExpression() { delete NumberOfElements; }

    ACCEPT_VISITOR
};
#endif // NEWINTARRAYEXPRESSION_H
