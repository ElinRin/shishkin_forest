#ifndef ARRAYMEMBEREXPRESSION_H
#define ARRAYMEMBEREXPRESSION_H
#include "common.h"

#include "Expression.h"

namespace AST {

struct ArrayMemberExpression : public IExpression
{
    std::unique_ptr<const IExpression> BaseExpression;
    std::unique_ptr<const IExpression> ElementNumberExpression;

    ArrayMemberExpression(Coordinates coords,
                          const IExpression* baseExpression,
                          const IExpression* elementNumberExpression) :
        BaseExpression(baseExpression),
        ElementNumberExpression(elementNumberExpression),
        coords(coords)
    {}

    ACCEPT_AST_VISITOR
};

}

#endif // ARRAYMEMBEREXPRESSION_H
