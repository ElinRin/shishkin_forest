#ifndef ACCEPTAST_VISITOR_CPP
#define ACCEPTAST_VISITOR_CPP

#include "Visitor.h"

namespace AST {

AST_VISIT_NODE(Program)
AST_VISIT_NODE(MainClass)
AST_VISIT_NODE(ClassDeclaration)
AST_VISIT_NODE(VarDeclaration)
AST_VISIT_NODE(MethodDeclaration)
AST_VISIT_NODE(Qualifier)
AST_VISIT_NODE(Type)

AST_VISIT_NODE(ReturnStatement)
AST_VISIT_NODE(AssignArrayElementStatement)
AST_VISIT_NODE(AssignStatement)
AST_VISIT_NODE(PrintLineStatement)
AST_VISIT_NODE(WhileStatement)
AST_VISIT_NODE(BraceSequenceStatement)
AST_VISIT_NODE(IfElseStatement)

AST_VISIT_NODE(BinaryExpression)
AST_VISIT_NODE(ArrayMemberExpression)
AST_VISIT_NODE(ArrayLengthExpression)
AST_VISIT_NODE(CallMemberExpression)
AST_VISIT_NODE(ValueExpression)
AST_VISIT_NODE(IdExpression)
AST_VISIT_NODE(ThisExpression)
AST_VISIT_NODE(NewIntArrayExpression)
AST_VISIT_NODE(NewObjectExpression)
AST_VISIT_NODE(NotExpression)
AST_VISIT_NODE(ContainerExpression)
AST_VISIT_NODE(Id)

}

#endif // ACCEPTAST_VISITOR_CPP
