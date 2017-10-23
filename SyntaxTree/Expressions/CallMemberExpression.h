#ifndef CALLMEMBEREXPRESSION_H
#define CALLMEMBEREXPRESSION_H
#include "common.h"

#include "Expression.h"

struct CallMemberExpression : public IExpression{
    const IExpression* BaseExpression;
    const Id* CalledMember;
    // Nullable
    const Sequence<const IExpression>* ArgumentSequence;

    CallMemberExpression(Coordinates coords, const IExpression* baseExpression,
                         const Id* calledMember,
                         const Sequence<const IExpression>* sequence) :
        coords(coords),
        BaseExpression(baseExpression),
        CalledMember(calledMember),
        ArgumentSequence(sequence) { }

    ~CallMemberExpression() {
        delete BaseExpression;
        delete CalledMember;
        delete ArgumentSequence;
    }

    ACCEPT_VISITOR
};
#endif // CALLMEMBEREXPRESSION_H
