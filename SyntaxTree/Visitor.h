#ifndef _VISITOR_H
#define _VISITOR_H
#include "common.h"

#include "NodeTypes.h"

#define VISITOR(TREE_INTERFACE) virtual void Visit(const TREE_INTERFACE* node) = 0;

#define VISIT_NODE(NODE_TYPE) void NODE_TYPE::AcceptVisitor(IVisitor* visitor) const {\
    visitor->Visit(this); \
}

namespace AST {

interface IVisitor{
    VISITOR(Program)
    VISITOR(MainClass)
    VISITOR(ClassDeclaration)
    VISITOR(VarDeclaration)
    VISITOR(MethodDeclaration)
    VISITOR(Sequence<const ClassDeclaration>)
    VISITOR(Sequence<const VarDeclaration>)
    VISITOR(Sequence<const MethodDeclaration>)
    VISITOR(Qualifier)
    VISITOR(Type)

    VISITOR(ReturnStatement)
    VISITOR(AssignArrayElementStatement)
    VISITOR(AssignStatement)
    VISITOR(PrintLineStatement)
    VISITOR(WhileStatement)
    VISITOR(BraceSequenceStatement)
    VISITOR(IfElseStatement)
    VISITOR(Sequence<const IStatement>)

    VISITOR(BinaryExpression)
    VISITOR(ArrayMemberExpression)
    VISITOR(ArrayLengthExpression)
    VISITOR(CallMemberExpression)
    VISITOR(Sequence<const IExpression>)
    VISITOR(ValueExpression)
    VISITOR(IdExpression)
    VISITOR(ThisExpression)
    VISITOR(NewIntArrayExpression)
    VISITOR(NewObjectExpression)
    VISITOR(NotExpression)
    VISITOR(ContainerExpression)
    VISITOR(Id)
};

}

#endif // _VISITOR_H
