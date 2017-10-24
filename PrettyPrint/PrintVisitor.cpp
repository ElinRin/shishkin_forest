#include "PrintVisitor.h"

#include <assert.h>

namespace AST {

void PrintVisitor::CreateGraph(Program* program)
{
    dot << "digraph g {\n" <<
           "graph [ rankdir = LR ];\n"
           "node [\n"
           "fontsize = \"16\"\n"
           "shape = \"ellipse\"\n"
           "];\n"
           "edge [\n"
           "];\n" << std::endl;
    program->AcceptVisitor(this);
    for(int i = 0; i < arrows.size(); ++i) {
        Arrow& arrow = arrows[i];
        dot << arrow.From << " -> " << arrow.To << " [\n" <<
               "id = " << i << "\n" <<
               "];" << std::endl;
    }
    dot << "}" << std::endl;
}

void PrintVisitor::Visit(const Program *node)
{
    std::string name = addNode(" Program | " + format(node->Coords()));
    parentName = name;
    node->Main->AcceptVisitor(this);
    parentName = name;
    node->Classes->AcceptVisitor(this);
}

void PrintVisitor::Visit(const MainClass *node)
{
    std::string name = addNode(" Main Class | " + node->ClassName->Name +
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->ClassName->AcceptVisitor(this);
    parentName = name;
    node->MainStatement->AcceptVisitor(this);
}

void PrintVisitor::Visit(const ClassDeclaration *node)
{
    std::string name = addNode(" Class | " + node->ClassName->Name +
                               (node->SuperName == nullptr ? "" : " | " + node->SuperName->Name) +
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->ClassName->AcceptVisitor(this);

    if(node->SuperName.get()) {
        parentName = name;
        node->SuperName->AcceptVisitor(this);
    }
    if(node->VarDeclarations.get()) {
        parentName = name;
        node->VarDeclarations->AcceptVisitor(this);
    }
    if(node->MethodDeclarations.get()) {
        parentName = name;
        node->MethodDeclarations->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const VarDeclaration *node)
{
    std::string name = addNode(" Var | " + format(node->VarType->TypeEnum) + " " +
                               node->Identifier->Name +
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->VarType->AcceptVisitor(this);
    parentName = name;
    node->Identifier->AcceptVisitor(this);
}

void PrintVisitor::Visit(const MethodDeclaration *node)
{
    std::string name = addNode(" Method | " + format(node->ReturnType->TypeEnum) + " " +
                               node->MethodName->Name +
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->QualifierType->AcceptVisitor(this);
    parentName = name;
    node->ReturnType->AcceptVisitor(this);
    parentName = name;
    node->MethodName->AcceptVisitor(this);
    if(node->Arguments.get()) {
        parentName = name;
        node->Arguments->AcceptVisitor(this);
    }
    if(node->VarDeclarations.get()) {
        parentName = name;
        node->VarDeclarations->AcceptVisitor(this);
    }
    if(node->Statements.get()) {
        parentName = name;
        node->Statements->AcceptVisitor(this);
    }
    parentName = name;
    node->StatementToReturn->AcceptVisitor(this);
}

void PrintVisitor::Visit(const Sequence<const ClassDeclaration> *node)
{
    std::string name = addNode(" Classes[] "
                               " | " + format(node->Coords()));
    addArrow(name);
    for(auto const& child : node->SequenceList) {
        parentName = name;
        child->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const Sequence<const VarDeclaration> *node)
{
    std::string name = addNode(" Vars[] "
                               " | " + format(node->Coords()));
    addArrow(name);
    for(auto const& child : node->SequenceList) {
        parentName = name;
        child->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const Sequence<const MethodDeclaration> *node)
{
    std::string name = addNode(" Methods[] "
                               " | " + format(node->Coords()));
    addArrow(name);
    for(auto const& child : node->SequenceList) {
        parentName = name;
        child->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const Qualifier *node)
{
    std::string name = addNode(" Qualifier | " + format(node->QualifierEnum) +
                               " | " + format(node->Coords()));
    addArrow(name);
}

void PrintVisitor::Visit(const Type *node)
{
    std::string name = addNode(" Type | " + format(node->TypeEnum) + " " +
                               (node->TypeName == nullptr ? "" : node->TypeName->Name) +
                               " | " + format(node->Coords()));
    addArrow(name);
    if(node->TypeName) {
        parentName = name;
        node->TypeName->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const ReturnStatement *node)
{
    std::string name = addNode(" return "
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->Expression->AcceptVisitor(this);
}

void PrintVisitor::Visit(const AssignArrayElementStatement *node)
{
    std::string name = addNode(" Assign | " + node->Identifier->Name + "[]" +
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->Identifier->AcceptVisitor(this);
    parentName = name;
    node->ElementNumberExpression->AcceptVisitor(this);
    parentName = name;
    node->ExpressionToAssign->AcceptVisitor(this);
}

void PrintVisitor::Visit(const AssignStatement *node)
{
    std::string name = addNode(" Assign | " + node->Identifier->Name +
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->Identifier->AcceptVisitor(this);
    parentName = name;
    node->ExpressionToAssign->AcceptVisitor(this);
}

void PrintVisitor::Visit(const PrintLineStatement *node)
{
    std::string name = addNode(" Println "
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->ExpressionToPrint->AcceptVisitor(this);
}

void PrintVisitor::Visit(const WhileStatement *node)
{
    std::string name = addNode(" While "
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->Condition->AcceptVisitor(this);
    parentName = name;
    node->Body->AcceptVisitor(this);
}

void PrintVisitor::Visit(const BraceSequenceStatement *node)
{
    std::string name = addNode(" Statements[] "
                               " | " + format(node->Coords()));
    addArrow(name);
    for(auto const& child : node->BracedSequence->SequenceList) {
        parentName = name;
        child->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const IfElseStatement *node)
{
    std::string name = addNode(" If Else "
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->Condition->AcceptVisitor(this);
    parentName = name;
    node->IfStatement->AcceptVisitor(this);
    parentName = name;
    node->ElseStatement->AcceptVisitor(this);
}

void PrintVisitor::Visit(const Sequence<const IStatement> *node)
{
    std::string name = addNode(" Statements[] "
                               " | " + format(node->Coords()));
    addArrow(name);
    for(auto const& child : node->SequenceList) {
        parentName = name;
        child->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const BinaryExpression *node)
{
    std::string name = addNode(" " + format(node->Type) +
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->Left->AcceptVisitor(this);
    parentName = name;
    node->Right->AcceptVisitor(this);
}

void PrintVisitor::Visit(const ArrayMemberExpression *node)
{
    std::string name = addNode(" [] "
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->BaseExpression->AcceptVisitor(this);
    parentName = name;
    node->ElementNumberExpression->AcceptVisitor(this);
}

void PrintVisitor::Visit(const ArrayLengthExpression *node)
{
    std::string name = addNode(" Length"
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->ArrayExpression->AcceptVisitor(this);
}

void PrintVisitor::Visit(const CallMemberExpression *node)
{
    std::string name = addNode(" ." + node->CalledMember->Name + "()"
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->BaseExpression->AcceptVisitor(this);
    parentName = name;
    node->CalledMember->AcceptVisitor(this);
    if(node->ArgumentSequence.get()) {
        parentName = name;
        node->ArgumentSequence->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const Sequence<const IExpression> *node)
{
    std::string name = addNode(" Expressions[] "
                               " | " + format(node->Coords()));
    addArrow(name);
    for(auto const& child : node->SequenceList) {
        parentName = name;
        child->AcceptVisitor(this);
    }
}

void PrintVisitor::Visit(const ValueExpression *node)
{
    std::string name = addNode(" Value  | " + format(node->ValueType) +
                               " | " + std::to_string(node->Value) +
                               " | " + format(node->Coords()));
    addArrow(name);
}

void PrintVisitor::Visit(const IdExpression *node)
{
    std::string name = addNode(" IdExp | " + node->ExpressionId->Name +
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->ExpressionId->AcceptVisitor(this);
}

void PrintVisitor::Visit(const ThisExpression *node)
{
    std::string name = addNode(" This"
                               " | " + format(node->Coords()));
    addArrow(name);
}

void PrintVisitor::Visit(const NewIntArrayExpression *node)
{
    std::string name = addNode(" new int[]"
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->NumberOfElements->AcceptVisitor(this);
}

void PrintVisitor::Visit(const NewObjectExpression *node)
{
    std::string name = addNode(" new " + node->ObjectId->Name +
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->ObjectId->AcceptVisitor(this);
}

void PrintVisitor::Visit(const NotExpression *node)
{
    std::string name = addNode(" not"
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->Expression->AcceptVisitor(this);
}

void PrintVisitor::Visit(const ContainerExpression *node)
{
    std::string name = addNode(" (Expression)"
                               " | " + format(node->Coords()));
    addArrow(name);
    parentName = name;
    node->Expression->AcceptVisitor(this);
}

void PrintVisitor::Visit(const Id *node)
{
    std::string name = addNode(" Id | " + node->Name +
                               " | " + format(node->Coords()));
    addArrow(name);
}

std::string PrintVisitor::addNode(std::string label)
{
    std::string name = "name" + std::to_string(nodeCounter);
    dot << "\"" << name << "\" [\n"
           "label = " << "\"<f0> " << label << "\"\n"
           "shape = \"record\"\n"
           "];" << std::endl;
    nodeCounter += 1;
    return name;
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

void PrintVisitor::addArrow(std::string &name)
{
    arrows.push_back({"\"" + parentName + "\"", "\"" + name + "\""});
}

}
