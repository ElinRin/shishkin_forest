#ifndef IDEXPRESSION_H
#define IDEXPRESSION_H
#include "common.h"

#include "Expression.h"
#include "Id.h"

namespace AST {

struct IdExpression : public IExpression {
    std::unique_ptr<const Id> ExpressionId;

    IdExpression(Coordinates coords, const Id* id) :
        ExpressionId(id),
        coords(coords)
    { }

    ACCEPT_AST_VISITOR
};

}

#endif // IDEXPRESSION_H
