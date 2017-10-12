#ifndef CALLMEMBEREXPRESSION_H
#define CALLMEMBEREXPRESSION_H
#include "common.h"

#include "Expression.h"
#include "ExpressionSequence.h"

struct CallMemberExpression : public IExpression{
    const IExpression* BaseExpression;
    // Nullable
    const ExpressionSequence* ArgumentSequence;

    CallMemberExpression(const IExpression* baseExpression,
                         const ExpressionSequence* sequence) :
        BaseExpression(baseExpression),
        ArgumentSequence(sequence) { }

    ~CallMemberExpression() {
        delete BaseExpression;
        delete ArgumentSequence;
    }

    ACCEPT_VISITOR
};
#endif // CALLMEMBEREXPRESSION_H
