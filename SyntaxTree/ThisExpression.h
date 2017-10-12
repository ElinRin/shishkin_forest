#ifndef THISEXPRESSION_H
#define THISEXPRESSION_H
#include "common.h"

#include "Expression.h"

struct ThisExpression : public IExpression
{
    ACCEPT_VISITOR
};
#endif // THISEXPRESSION_H
