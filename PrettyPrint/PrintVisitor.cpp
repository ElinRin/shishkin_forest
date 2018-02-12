#include "PrintVisitor.h"

#include <assert.h>

namespace AST {

void PrintVisitor::CreateGraph(Program* program)
{
    Dot << "digraph g {\n" <<
           "graph [ rankdir = LR ];\n"
           "node [\n"
           "fontsize = \"16\"\n"
           "shape = \"ellipse\"\n"
           "];\n"
           "edge [\n"
           "];\n" << std::endl;
    program->AcceptVisitor(this);
    PrintArrows();
    Dot << "}" << std::endl;
}

void PrintVisitor::Visit(const Program *node)
{
    std::string name = AddNode(" Program | " + format(node->Coords()));
    ParentName = name;
    node->Main->AcceptVisitor(this);
    ParentName = name;
    node->Classes->AcceptVisitor(this);
}

void PrintVisitor::Visit(const MainClass *node)
{
    std::string name = AddNode(" Main Class | " + node->ClassName->Name +
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->ClassName->AcceptVisitor(this);
    ParentName = name;
    node->MainStatement->AcceptVisitor(this);
}

void PrintVisitor::Visit(const ClassDeclaration *node)
{
    std::string name = AddNode(" Class | " + node->ClassName->Name +
                               (node->SuperName == nullptr ? "" : " | " + node->SuperName->Name) +
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->ClassName->AcceptVisitor(this);

    if(node->SuperName.get()) {
        ParentName = name;
        node->SuperName->AcceptVisitor(this);
    }
    if(node->VarDeclarations.get()) {
        ParentName = name;
        node->VarDeclarations->AcceptVisitor(this);
    }
    if(node->MethodDeclarations.get()) {
        ParentName = name;
        node->MethodDeclarations->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const VarDeclaration *node)
{
    std::string name = AddNode(" Var | " + format(node->VarType->TypeEnum) + " " +
                               node->Identifier->Name +
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->VarType->AcceptVisitor(this);
    ParentName = name;
    node->Identifier->AcceptVisitor(this);
}

void PrintVisitor::Visit(const MethodDeclaration *node)
{
    std::string name = AddNode(" Method | " + format(node->ReturnType->TypeEnum) + " " +
                               node->MethodName->Name +
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->QualifierType->AcceptVisitor(this);
    ParentName = name;
    node->ReturnType->AcceptVisitor(this);
    ParentName = name;
    node->MethodName->AcceptVisitor(this);
    if(node->Arguments.get()) {
        ParentName = name;
        node->Arguments->AcceptVisitor(this);
    }
    if(node->VarDeclarations.get()) {
        ParentName = name;
        node->VarDeclarations->AcceptVisitor(this);
    }
    if(node->Statements.get()) {
        ParentName = name;
        node->Statements->AcceptVisitor(this);
    }
    ParentName = name;
    node->StatementToReturn->AcceptVisitor(this);
}

void PrintVisitor::Visit(const Sequence<const ClassDeclaration> *node)
{
    std::string name = AddNode(" Classes[] "
                               " | " + format(node->Coords()));
    AddArrow(name);
    for(auto const& child : node->SequenceList) {
        ParentName = name;
        child->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const Sequence<const VarDeclaration> *node)
{
    std::string name = AddNode(" Vars[] "
                               " | " + format(node->Coords()));
    AddArrow(name);
    for(auto const& child : node->SequenceList) {
        ParentName = name;
        child->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const Sequence<const MethodDeclaration> *node)
{
    std::string name = AddNode(" Methods[] "
                               " | " + format(node->Coords()));
    AddArrow(name);
    for(auto const& child : node->SequenceList) {
        ParentName = name;
        child->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const Qualifier *node)
{
    std::string name = AddNode(" Qualifier | " + format(node->QualifierEnum) +
                               " | " + format(node->Coords()));
    AddArrow(name);
}

void PrintVisitor::Visit(const Type *node)
{
    std::string name = AddNode(" Type | " + format(node->TypeEnum) + " " +
                               (node->TypeName == nullptr ? "" : node->TypeName->Name) +
                               " | " + format(node->Coords()));
    AddArrow(name);
    if(node->TypeName) {
        ParentName = name;
        node->TypeName->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const ReturnStatement *node)
{
    std::string name = AddNode(" return "
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->Expression->AcceptVisitor(this);
}

void PrintVisitor::Visit(const AssignArrayElementStatement *node)
{
    std::string name = AddNode(" Assign | " + node->Identifier->Name + "[]" +
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->Identifier->AcceptVisitor(this);
    ParentName = name;
    node->ElementNumberExpression->AcceptVisitor(this);
    ParentName = name;
    node->ExpressionToAssign->AcceptVisitor(this);
}

void PrintVisitor::Visit(const AssignStatement *node)
{
    std::string name = AddNode(" Assign | " + node->Identifier->Name +
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->Identifier->AcceptVisitor(this);
    ParentName = name;
    node->ExpressionToAssign->AcceptVisitor(this);
}

void PrintVisitor::Visit(const PrintLineStatement *node)
{
    std::string name = AddNode(" Println "
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->ExpressionToPrint->AcceptVisitor(this);
}

void PrintVisitor::Visit(const WhileStatement *node)
{
    std::string name = AddNode(" While "
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->Condition->AcceptVisitor(this);
    ParentName = name;
    node->Body->AcceptVisitor(this);
}

void PrintVisitor::Visit(const BraceSequenceStatement *node)
{
    std::string name = AddNode(" Statements[] "
                               " | " + format(node->Coords()));
    AddArrow(name);
    for(auto const& child : node->BracedSequence->SequenceList) {
        ParentName = name;
        child->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const IfElseStatement *node)
{
    std::string name = AddNode(" If Else "
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->Condition->AcceptVisitor(this);
    ParentName = name;
    node->IfStatement->AcceptVisitor(this);
    ParentName = name;
    node->ElseStatement->AcceptVisitor(this);
}

void PrintVisitor::Visit(const Sequence<const IStatement> *node)
{
    std::string name = AddNode(" Statements[] "
                               " | " + format(node->Coords()));
    AddArrow(name);
    for(auto const& child : node->SequenceList) {
        ParentName = name;
        child->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const BinaryExpression *node)
{
    std::string name = AddNode(" " + format(node->Type) +
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->Left->AcceptVisitor(this);
    ParentName = name;
    node->Right->AcceptVisitor(this);
}

void PrintVisitor::Visit(const ArrayMemberExpression *node)
{
    std::string name = AddNode(" [] "
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->BaseExpression->AcceptVisitor(this);
    ParentName = name;
    node->ElementNumberExpression->AcceptVisitor(this);
}

void PrintVisitor::Visit(const ArrayLengthExpression *node)
{
    std::string name = AddNode(" Length"
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->ArrayExpression->AcceptVisitor(this);
}

void PrintVisitor::Visit(const CallMemberExpression *node)
{
    std::string name = AddNode(" ." + node->CalledMember->Name + "()"
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->BaseExpression->AcceptVisitor(this);
    ParentName = name;
    node->CalledMember->AcceptVisitor(this);
    if(node->ArgumentSequence.get()) {
        ParentName = name;
        node->ArgumentSequence->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const Sequence<const IExpression> *node)
{
    std::string name = AddNode(" Expressions[] "
                               " | " + format(node->Coords()));
    AddArrow(name);
    for(auto const& child : node->SequenceList) {
        ParentName = name;
        child->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const ValueExpression *node)
{
    std::string name = AddNode(" Value  | " + format(node->ValueType) +
                               " | " + std::to_string(node->Value) +
                               " | " + format(node->Coords()));
    AddArrow(name);
}

void PrintVisitor::Visit(const IdExpression *node)
{
    std::string name = AddNode(" IdExp | " + node->ExpressionId->Name +
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->ExpressionId->AcceptVisitor(this);
}

void PrintVisitor::Visit(const ThisExpression *node)
{
    std::string name = AddNode(" This"
                               " | " + format(node->Coords()));
    AddArrow(name);
}

void PrintVisitor::Visit(const NewIntArrayExpression *node)
{
    std::string name = AddNode(" new int[]"
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->NumberOfElements->AcceptVisitor(this);
}

void PrintVisitor::Visit(const NewObjectExpression *node)
{
    std::string name = AddNode(" new " + node->ObjectId->Name +
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->ObjectId->AcceptVisitor(this);
}

void PrintVisitor::Visit(const NotExpression *node)
{
    std::string name = AddNode(" not"
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->Expression->AcceptVisitor(this);
}

void PrintVisitor::Visit(const ContainerExpression *node)
{
    std::string name = AddNode(" (Expression)"
                               " | " + format(node->Coords()));
    AddArrow(name);
    ParentName = name;
    node->Expression->AcceptVisitor(this);
}

void PrintVisitor::Visit(const Id *node)
{
    std::string name = AddNode(" Id | " + node->Name +
                               " | " + format(node->Coords()));
    AddArrow(name);
}

std::string PrintVisitor::format(const Coordinates &coords)
{
    return "(" + std::to_string(coords.Row) + "," + std::to_string(coords.Column) + ")";
}

std::string PrintVisitor::format(const T_Type &type)
{
    switch(type) {
        case T_Int:
            return "int";
        case T_IntArray:
            return "int[]";
        case T_Boolean:
            return "boolean";
        case T_UserType:
            return "object";
        default:
            assert(false);
    }
}

std::string PrintVisitor::format(const T_Qualifier &type)
{
    switch (type) {
    case Q_Public:
        return "public";
    case Q_Private:
        return "private";
    default:
        assert(false);
    }
}

std::string PrintVisitor::format(const T_BinaryExpressionType &type)
{
    switch (type) {
    case BET_And:
        return "&&";
    case BET_Or:
        return "||";
    case BET_Plus:
        return "+";
    case BET_Minus:
        return "-";
    case BET_Mult:
        return "*";
    case BET_Mod:
        return "%";
    case BET_Less:
        return "\\<";
    default:
        assert(false);
    }
}

std::string PrintVisitor::format(const T_ValueType &type)
{
    switch (type) {
    case VT_Int:
        return "int";
    case VT_Boolean:
        return "boolean";
    default:
        assert(false);
    }
}

}
