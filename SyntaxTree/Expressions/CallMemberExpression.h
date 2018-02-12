#ifndef CALLMEMBEREXPRESSION_H
#define CALLMEMBEREXPRESSION_H
#include "common.h"

#include "Expression.h"

namespace AST {

struct CallMemberExpression : public IExpression{
    std::unique_ptr<const IExpression> BaseExpression;
    std::unique_ptr<const Id> CalledMember;
    // Nullable
    std::unique_ptr<const Sequence<const IExpression>> ArgumentSequence;

    CallMemberExpression(Coordinates coords, const IExpression* baseExpression,
                         const Id* calledMember,
                         const Sequence<const IExpression>* sequence) :
        BaseExpression(baseExpression),
        CalledMember(calledMember),
        ArgumentSequence(sequence),
        coords(coords)
    { }

    ACCEPT_AST_VISITOR
};

}
#endif // CALLMEMBEREXPRESSION_H
