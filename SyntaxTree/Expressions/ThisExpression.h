#ifndef THISEXPRESSION_H
#define THISEXPRESSION_H
#include "common.h"

#include "Expression.h"

struct ThisExpression : public IExpression
{
    ThisExpression(Coordinates coords) : coords(coords) {}
    ACCEPT_VISITOR
};
#endif // THISEXPRESSION_H
