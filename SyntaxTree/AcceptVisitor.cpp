#ifndef ACCEPTVISITOR_CPP
#define ACCEPTVISITOR_CPP

#include "Visitor.h"

namespace AST {

VISIT_NODE(Program)
VISIT_NODE(MainClass)
VISIT_NODE(ClassDeclaration)
VISIT_NODE(VarDeclaration)
VISIT_NODE(MethodDeclaration)
VISIT_NODE(Qualifier)
VISIT_NODE(Type)

VISIT_NODE(ReturnStatement)
VISIT_NODE(AssignArrayElementStatement)
VISIT_NODE(AssignStatement)
VISIT_NODE(PrintLineStatement)
VISIT_NODE(WhileStatement)
VISIT_NODE(BraceSequenceStatement)
VISIT_NODE(IfElseStatement)

VISIT_NODE(BinaryExpression)
VISIT_NODE(ArrayMemberExpression)
VISIT_NODE(ArrayLengthExpression)
VISIT_NODE(CallMemberExpression)
VISIT_NODE(ValueExpression)
VISIT_NODE(IdExpression)
VISIT_NODE(ThisExpression)
VISIT_NODE(NewIntArrayExpression)
VISIT_NODE(NewObjectExpression)
VISIT_NODE(NotExpression)
VISIT_NODE(ContainerExpression)
VISIT_NODE(Id)

}

#endif // ACCEPTVISITOR_CPP
