#ifndef _AST_VISITOR_H
#define _AST_VISITOR_H
#include "common.h"

#include "NodeTypes.h"

#define AST_VISITOR(TREE_INTERFACE) virtual void Visit(const TREE_INTERFACE* node) = 0;

#define AST_VISIT_NODE(NODE_TYPE) void NODE_TYPE::AcceptVisitor(IVisitor* Visitor) const {\
    Visitor->Visit(this); \
}

namespace AST {

interface IVisitor{
    AST_VISITOR(Program)
    AST_VISITOR(MainClass)
    AST_VISITOR(ClassDeclaration)
    AST_VISITOR(VarDeclaration)
    AST_VISITOR(MethodDeclaration)
    AST_VISITOR(Sequence<const ClassDeclaration>)
    AST_VISITOR(Sequence<const VarDeclaration>)
    AST_VISITOR(Sequence<const MethodDeclaration>)
    AST_VISITOR(Qualifier)
    AST_VISITOR(Type)

    AST_VISITOR(ReturnStatement)
    AST_VISITOR(AssignArrayElementStatement)
    AST_VISITOR(AssignStatement)
    AST_VISITOR(PrintLineStatement)
    AST_VISITOR(WhileStatement)
    AST_VISITOR(BraceSequenceStatement)
    AST_VISITOR(IfElseStatement)
    AST_VISITOR(Sequence<const IStatement>)

    AST_VISITOR(BinaryExpression)
    AST_VISITOR(ArrayMemberExpression)
    AST_VISITOR(ArrayLengthExpression)
    AST_VISITOR(CallMemberExpression)
    AST_VISITOR(Sequence<const IExpression>)
    AST_VISITOR(ValueExpression)
    AST_VISITOR(IdExpression)
    AST_VISITOR(ThisExpression)
    AST_VISITOR(NewIntArrayExpression)
    AST_VISITOR(NewObjectExpression)
    AST_VISITOR(NotExpression)
    AST_VISITOR(ContainerExpression)
    AST_VISITOR(Id)
};

}

#endif // _AST_VISITOR_H
