#include "IRBuilder.h"

#include "IRNodeTypes.h"
#include "ExpWrapper.h"
#include "StmWrapper.h"

using namespace IR;

namespace IRTranslate {

static std::string PrintLnName = "println";

namespace  {

SymbolTable::Position toPosition(const AST::Coordinates& coords) {
    return SymbolTable::Position(coords.Column, coords.Row);
}

}

void IRBuilder::Visit(const AST::Program* node)
{
    node->Main->AcceptVisitor(this);
    node->Classes->AcceptVisitor(this);
}

void IRBuilder::Visit(const AST::MainClass* node)
{
    node->MainStatement->AcceptVisitor(this);
}

void IRBuilder::Visit(const AST::ClassDeclaration* node)
{
    node->MethodDeclarations->AcceptVisitor(this);
}

void IRBuilder::Visit(const AST::MethodDeclaration* node)
{
    node->Statements->AcceptVisitor(this);
    auto methodInfo = table->GetMethod(node->MethodName->Name,
                                       SymbolTable::Position(toPosition(node->Coords())));
    auto name = methodInfo->GetFullName();
    currentFrame = methodInfo->GetFrame();
    trees.insert(std::make_pair(name, std::unique_ptr<ISubtreeWrapper>(mainSubtree.release())));
}

void IRBuilder::Visit(const AST::Sequence<const AST::ClassDeclaration>* node)
{
    for(auto const& classToParse: node->SequenceList) {
        classToParse->AcceptVisitor(this);
    }
}

void IRBuilder::Visit(const AST::Sequence<const AST::MethodDeclaration>* node)
{
    for(auto const& method: node->SequenceList) {
        method->AcceptVisitor(this);
    }
}

void IRBuilder::Visit(const AST::ReturnStatement* node)
{
    node->Expression->AcceptVisitor(this);
    std::string fpName = "fp";
    const IExp* returnAddress = currentFrame->ExitAddress()->GetExp(new Temp(fpName));
    mainSubtree.reset(new StmWrapper(new Move(returnAddress, mainSubtree->ToExp())));
}

void IRBuilder::Visit(const AST::AssignArrayElementStatement* node)
{
    node->ElementNumberExpression->AcceptVisitor(this);
    std::unique_ptr<ISubtreeWrapper> elementNumberExp(mainSubtree.release());
    node->ExpressionToAssign->AcceptVisitor(this);
    std::string fpName = "fp";
    auto identifierName = StringSymbol::GetIntern(node->Identifier->Name);
    const AR::IAccess* access = currentFrame->FindLocalOrFormal(identifierName);
    const IExp* baseAddresss = access->GetExp(new Temp(fpName));
    IExp* address = new Mem(new Binop(Binop::TB_PLUS, baseAddresss,
                                      new Binop(Binop::TB_MUL,
                                                new Const(currentFrame->TypeSize(currentFrame->WordType().GetType())),
                                                elementNumberExp->ToExp())));
    mainSubtree.reset(new StmWrapper(new Move(address, mainSubtree.release()->ToExp())));
}

void IRBuilder::Visit(const AST::AssignStatement *node)
{
    node->ExpressionToAssign->AcceptVisitor(this);
    auto identifierName = StringSymbol::GetIntern(node->Identifier->Name);
    const AR::IAccess* access = currentFrame->FindLocalOrFormal(identifierName);
    std::string fpName = "fp";
    mainSubtree.reset(new StmWrapper(new Move(access->GetExp(new Temp(fpName)), mainSubtree.release()->ToExp())));
}

void IRBuilder::Visit(const AST::PrintLineStatement* node)
{
    node->ExpressionToPrint->AcceptVisitor(this);
    mainSubtree.reset(new ExpWrapper(new Call(new Name(PrintLnName), new ExpList(mainSubtree.release()->ToExp()))));
}

void IRBuilder::Visit(const AST::WhileStatement* node)
{
    node->Condition->AcceptVisitor(this);
    Label* conditionLabel = Label::GetNextEnumeratedLabel();
    Label* bodyLabel = Label::GetNextEnumeratedLabel();
    Label* exitLabel = Label::GetNextEnumeratedLabel();
    const IStm* condition = mainSubtree.release()->ToConditional(bodyLabel, exitLabel);
    const IStm* conditionPart = new Seq(new LabelStm(conditionLabel), condition);
    node->Body->AcceptVisitor(this);
    const IStm* bodyPart = new Seq(new LabelStm(bodyLabel), new Seq(mainSubtree.release()->ToStm(), new Jump(conditionLabel)));
    mainSubtree.reset(new StmWrapper(new Seq(new Seq(conditionPart, bodyPart), new LabelStm(exitLabel))));
}

void IRBuilder::Visit(const AST::BraceSequenceStatement* node)
{
    node->BracedSequence->AcceptVisitor(this);
}

void IRBuilder::Visit(const AST::IfElseStatement* node)
{
    node->Condition->AcceptVisitor(this);
    Label* ifBranchLabel = Label::GetNextEnumeratedLabel();
    Label* elseBranchLabel = Label::GetNextEnumeratedLabel();
    Label* exitLabel = Label::GetNextEnumeratedLabel();
    const IStm* condition = mainSubtree.release()->ToConditional(ifBranchLabel, elseBranchLabel);
    node->IfStatement->AcceptVisitor(this);
    const IStm* ifPart = new Seq(new Seq(new LabelStm(ifBranchLabel), mainSubtree.release()->ToStm()), new Jump(exitLabel));
    node->ElseStatement->AcceptVisitor(this);
    const IStm* elsePart = new Seq(new Seq(new LabelStm(elseBranchLabel), mainSubtree.release()->ToStm()), new Jump(exitLabel));
    mainSubtree.reset(new StmWrapper(new Seq(new Seq(condition, new Seq(ifPart, elsePart)), new LabelStm(exitLabel))));
}

void IRBuilder::Visit(const AST::Sequence<const AST::IStatement>* node)
{
    std::vector<const IStm*> statements;
    for(auto const& statement: node->SequenceList) {
        statement->AcceptVisitor(this);
        statements.push_back(mainSubtree.release()->ToStm());
    }
    assert(statements.size() > 0);
    if(statements.size() == 1) {
        mainSubtree.reset(new StmWrapper(statements[0]));
    } else {
        IStm* seq = new Seq(statements[0], statements[1]);
        for(int i = 2; i < statements.size(); ++i) {
            seq = new Seq(seq, statements[i]);
        }
        mainSubtree.reset(new StmWrapper(seq));
    }
}

void IRBuilder::Visit(const AST::BinaryExpression* node)
{
    const IExp* result;
    node->Left->AcceptVisitor(this);
    const IExp* left = mainSubtree.release()->ToExp();
    node->Right->AcceptVisitor(this);
    const IExp* right = mainSubtree.release()->ToExp();
    switch (node->Type) {
    case AST::BET_And:
        result = new Binop(Binop::TB_AND, left, right);
        break;
    case AST::BET_Minus:
        result = new Binop(Binop::TB_MINUS, left, right);
        break;
    case AST::BET_Mult:
        result = new Binop(Binop::TB_MUL, left, right);
        break;
    case AST::BET_Plus:
        result = new Binop(Binop::TB_PLUS, left, right);
        break;
    default:
        break;
    }
}

}
