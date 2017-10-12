#ifndef ACCEPTVISITOR_CPP
#define ACCEPTVISITOR_CPP

#include "Visitor.h"

#define VISIT_NODE(NODE_TYPE) void NODE_TYPE::AcceptVisitor(IVisitor *visitor) const {\
    visitor->Visit(this); \
}

VISIT_NODE(BinaryExpression)
VISIT_NODE(ArrayMemberExpression)
VISIT_NODE(ArrayLengthExpression)
VISIT_NODE(CallMemberExpression)
VISIT_NODE(ExpressionSequence)
VISIT_NODE(ValueExpression)
VISIT_NODE(IdExpression)
VISIT_NODE(ThisExpression)
VISIT_NODE(NewIntArrayExpression)
VISIT_NODE(NewObjectExpression)
VISIT_NODE(NotExpression)
VISIT_NODE(ContainerExpression)
VISIT_NODE(Id)


#endif // ACCEPTVISITOR_CPP
