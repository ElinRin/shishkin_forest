#include "IRPrinter.h"

namespace IRTranslate {

void IRPrinter::CreateGraph(IRForest& forest)
{
    Dot << "digraph g {\n" <<
           "graph [ rankdir = LR ];\n"
           "node [\n"
           "fontsize = \"16\"\n"
           "shape = \"ellipse\"\n"
           "];\n"
           "edge [\n"
           "];\n" << std::endl;
    for(auto&& tree : forest) {
        std::string name = AddNode(" Method | " + tree.first->GetString());
        ParentName = name;
        tree.second->AcceptVisitor(this);
    }
    PrintArrows();
    Dot << "}" << std::endl;
}

void IRPrinter::CreateGraph(IRLinearForest& forest)
{
    Dot << "digraph g {\n" <<
           "graph [ rankdir = LR ];\n"
           "node [\n"
           "fontsize = \"16\"\n"
           "shape = \"ellipse\"\n"
           "];\n"
           "edge [\n"
           "];\n" << std::endl;
    for(auto&& tree : forest) {
        std::string name = AddNode(" Method | " + tree.first->GetString());
        ParentName = name;
        for(auto&& stm: tree.second) {
            stm->AcceptVisitor(this);
            ParentName = name;
        }
    }
    PrintArrows();
    Dot << "}" << std::endl;
}

void IRPrinter::Visit(IR::Unaryop* node)
{
    std::string name = AddNode("Unary | " + format(node->Operation) + " | " + format(node->GetCoords()));
    AddArrow(name);
    ParentName = name;
    node->Expression->AcceptVisitor(this);
}

void IRPrinter::Visit(IR::Binop* node)
{
    std::string name = AddNode("Binary | " + format(node->Operation) + " | " + format(node->GetCoords()));
    AddArrow(name);
    ParentName = name;
    node->LeftExpression->AcceptVisitor(this);
    ParentName = name;
    node->RightExpression->AcceptVisitor(this);
}

void IRPrinter::Visit(IR::Call* node)
{
    std::string name = AddNode("Call | " + format(node->GetCoords()));
    AddArrow(name);
    ParentName = name;
    node->Arguments->AcceptVisitor(this);
    ParentName = name;
    node->FuncExpression->AcceptVisitor(this);
}

void IRPrinter::Visit(IR::Const* node)
{
    std::string name = AddNode("Const | " + std::to_string(node->value) + " | " + format(node->GetCoords()));
    AddArrow(name);
}

void IRPrinter::Visit(IR::Eseq* node)
{
    std::string name = AddNode("Eseq | " + format(node->GetCoords()));
    AddArrow(name);
    ParentName = name;
    node->Statement->AcceptVisitor(this);
    ParentName = name;
    node->Expression->AcceptVisitor(this);
}

void IRPrinter::Visit(IR::Mem* node)
{
    std::string name = AddNode("Mem | " + format(node->GetCoords()));
    AddArrow(name);
    ParentName = name;
    node->Expression->AcceptVisitor(this);
}

void IRPrinter::Visit(IR::Name* node)
{
    std::string name = AddNode("Name | " + node->LabelName->GetName() + " | " + format(node->GetCoords()));
    AddArrow(name);
}

void IRPrinter::Visit(IR::Temp* node)
{
    std::string name = AddNode("Temp | " + format(node) + " | " + format(node->GetCoords()));
    AddArrow(name);
}

void IRPrinter::Visit(IR::Exp* node)
{
    std::string name = AddNode("Exp | " + format(node->GetCoords()));
    AddArrow(name);
    ParentName = name;
    node->expression->AcceptVisitor(this);
}

void IRPrinter::Visit(IR::Jump* node)
{
    std::string name = AddNode("Jump | " + node->LabelToJump->GetName() + " | " + format(node->GetCoords()));
    AddArrow(name);
}

void IRPrinter::Visit(IR::JumpC* node)
{
    std::string name = AddNode("JumpC | " + format(node->JumpType) +
                               " | True: " + node->TrueLabel->GetName() +
                               " | " + format(node->GetCoords()));
    AddArrow(name);
    ParentName = name;
    node->ConditionLeftExpression->AcceptVisitor(this);
    ParentName = name;
    node->ConditionRightExpression->AcceptVisitor(this);
}

void IRPrinter::Visit(IR::LabelStm* node)
{
    std::string name = AddNode("LabelStm | " + node->LabelName->GetName() + " | " + format(node->GetCoords()));
    AddArrow(name);
}

void IRPrinter::Visit(IR::Move* node)
{
    std::string name = AddNode("Move | " + format(node->GetCoords()));
    AddArrow(name);
    ParentName = name;
    node->Source->AcceptVisitor(this);
    ParentName = name;
    node->Destination->AcceptVisitor(this);
}

void IRPrinter::Visit(IR::ExpList* node)
{
    std::string name = AddNode("ExpList | " + format(node->GetCoords()));
    AddArrow(name);
    if(node->Head != nullptr) {
        ParentName = name;
        node->Head->AcceptVisitor(this);
    }
    if(node->Tail != nullptr) {
        ParentName = name;
        node->Tail->AcceptVisitor(this);
    }
}

void IRPrinter::Visit(IR::StmList* node)
{
    std::string name = AddNode("Seq | " + format(node->GetCoords()));
    AddArrow(name);
    if(node->Head != nullptr) {
        ParentName = name;
        node->Head->AcceptVisitor(this);
    }
    if(node->Tail != nullptr) {
        ParentName = name;
        node->Tail->AcceptVisitor(this);
    }
}

void IRPrinter::Visit(StmWrapper* node)
{
    node->Stm()->AcceptVisitor(this);
}

void IRPrinter::Visit(ExpWrapper *node)
{
    node->Exp()->AcceptVisitor(this);
}

std::string IRPrinter::format(IR::Unaryop::TUnaryop op)
{
    switch (op) {
    case IR::Unaryop::TU_NOT:
        return "Not";
    default:
        assert(false);
        break;
    }
}

std::string IRPrinter::format(IR::Binop::TBinop op)
{
    switch (op) {
    case IR::Binop::TB_AND:
        return "AND";
    case IR::Binop::TB_MINUS:
        return "MINUS";
    case IR::Binop::TB_MOD:
        return "MOD";
    case IR::Binop::TB_MUL:
        return "MULT";
    case IR::Binop::TB_OR:
        return "OR";
    case IR::Binop::TB_PLUS:
        return "PLUS";
    default:
        assert(false);
        return "";
    }
}

std::string IRPrinter::format(const IR::Temp* temp)
{
    assert(temp != nullptr);
    std::string name = " ID: " + std::to_string(temp->Id) + " | ";
    switch (temp->Info()) {
    case IR::Temp::AI_Id:
        return name + std::to_string(temp->LocalId());
    case IR::Temp::AI_Name:
        return name + temp->Name();
    default:
        assert(false);
        return "";
    }
}

std::string IRPrinter::format(IR::JumpC::TJumpType type)
{
    switch(type) {
    case IR::JumpC::TJ_EQ:
        return "==";
    case IR::JumpC::TJ_LT:
        return "\\<";
    case IR::JumpC::TJ_NEQ:
        return "!=";
    default:
        assert(false);
        break;
    }
}

std::string IRPrinter::format(const IR::Coords& coords)
{
    return coords.ToString();
}

}
