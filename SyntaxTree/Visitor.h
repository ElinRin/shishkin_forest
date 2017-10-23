#ifndef _VISITOR_H
#define _VISITOR_H
#include "common.h"

#include "NodeTypes.h"

#define VISITOR(TREE_INTERFACE) virtual void Visit(const TREE_INTERFACE* node) = 0;

interface IVisitor{
    VISITOR(Program)
    VISITOR(MainClass)
    VISITOR(ClassDeclaration)
    VISITOR(VarDeclaration)
    VISITOR(MethodDeclaration)
    VISITOR(Sequence<ClassDeclaration>)
    VISITOR(Sequence<VarDeclaration>)
    VISITOR(Sequence<MethodDeclaration>)
    VISITOR(Type)

    VISITOR(ReturnStatement)
    VISITOR(AssignArrayElementStatement)
    VISITOR(AssignStatement)
    VISITOR(PrintLineStatement)
    VISITOR(WhileStatement)
    VISITOR(BraceSequenceStatement)
    VISITOR(IfElseStatement)
    VISITOR(Sequence<IStatement>)

    VISITOR(BinaryExpression)
    VISITOR(ArrayMemberExpression)
    VISITOR(ArrayLengthExpression)
    VISITOR(CallMemberExpression)
    VISITOR(Sequence<IExpression>)
    VISITOR(ValueExpression)
    VISITOR(IdExpression)
    VISITOR(ThisExpression)
    VISITOR(NewIntArrayExpression)
    VISITOR(NewObjectExpression)
    VISITOR(NotExpression)
    VISITOR(ContainerExpression)
    VISITOR(Id)
};


#endif // _VISITOR_H
