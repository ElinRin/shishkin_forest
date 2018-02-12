#include "IRBuilder.h"

#include "IRNodeTypes.h"
#include "ExpWrapper.h"
#include "StmWrapper.h"
#include "ArrayStruct.h"
#include "ClassStruct.h"
#include "NameConventions.h"

using namespace IR;

namespace IRTranslate {

static std::string PrintLnName = "println";
static std::string FpName = "fp";

namespace  {

SymbolTable::Position toPosition(const AST::Coordinates& coords) {
    return SymbolTable::Position(coords.Column, coords.Row);
}

}

void IRBuilder::Parse(const AST::Program* program)
{
    program->AcceptVisitor(this);
}

void IRBuilder::Visit(const AST::Program* node)
{
    node->Main->AcceptVisitor(this);
    node->Classes->AcceptVisitor(this);
}

void IRBuilder::Visit(const AST::MainClass* node)
{
    node->MainStatement->AcceptVisitor(this);
    assert(mainSubtree.get() != nullptr);
    trees.insert(std::make_pair(StringSymbol::GetIntern(node->ClassName->Name+"@MAIN"),
                                std::unique_ptr<ISubtreeWrapper>(mainSubtree.release())));
}

void IRBuilder::Visit(const AST::ClassDeclaration* node)
{
    SymbolTable::TypeScopeSwitcher switcher(table->GetClass(node->ClassName->Name)->GetTypeInfo(),
                                   table, toPosition(node->Coords()));
    node->MethodDeclarations->AcceptVisitor(this);
}

void IRBuilder::Visit(const AST::MethodDeclaration* node)
{
    auto pos = SymbolTable::Position(toPosition(node->Coords()));
    auto methodInfo = table->GetMethod(node->MethodName->Name, pos);
    SymbolTable::MethodScopeSwitcher switcher(methodInfo, table, pos);
    currentFrame = methodInfo->GetFrame();
    StmList* stm = nullptr;
    if( node->Statements != nullptr ) {
        node->Statements->AcceptVisitor(this);
        stm = new StmList(mainSubtree->ToStm());
    }
    node->StatementToReturn->AcceptVisitor(this);
    stm = new StmList(mainSubtree->ToStm(), stm);
    auto name = methodInfo->GetFullName();
    trees.insert(std::make_pair(name, std::unique_ptr<ISubtreeWrapper>(new StmWrapper(stm))));
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
    TypeStackVisitor.PopTypeFromStack();
    const IExp* returnAddress = currentFrame->ExitAddress()->GetExp(new Temp(FpName));
    mainSubtree.reset(new StmWrapper(new Move(returnAddress, mainSubtree->ToExp())));
}

void IRBuilder::Visit(const AST::AssignArrayElementStatement* node)
{
    node->ElementNumberExpression->AcceptVisitor(this);
    TypeStackVisitor.PopTypeFromStack();
    std::unique_ptr<ISubtreeWrapper> elementNumberExp(mainSubtree.release());
    node->ExpressionToAssign->AcceptVisitor(this);
    auto identifierName = StringSymbol::GetIntern(node->Identifier->Name);
    const AR::IAccess* access = currentFrame->FindLocalOrFormal(identifierName);
    const IExp* baseAddresss = nullptr;
    if(access != nullptr) {
        baseAddresss = access->GetExp(new Temp(FpName));
    } else {
        baseAddresss = table->GetScopedClass()->GetClassStruct()->GetFieldFrom(
                    identifierName,
                    currentFrame->FindLocalOrFormal(
                            StringSymbol::GetIntern(NameConventions::ThisName)
                        )->GetExp(new Temp(FpName)));
    }
    IExp* address = new Mem(new Binop(Binop::TB_PLUS, baseAddresss,
                                      new Binop(Binop::TB_MUL,
                                                new Const(currentFrame->TypeSize(currentFrame->WordType().GetType())),
                                                elementNumberExp->ToExp())));
    mainSubtree.reset(new StmWrapper(new Move(address, mainSubtree->ToExp())));
}

void IRBuilder::Visit(const AST::AssignStatement *node)
{
    node->ExpressionToAssign->AcceptVisitor(this);
    auto identifierName = StringSymbol::GetIntern(node->Identifier->Name);
    const AR::IAccess* access = currentFrame->FindLocalOrFormal(identifierName);
    const IExp* baseAddresss = nullptr;
    //CP
    if(access != nullptr) {
        baseAddresss = access->GetExp(new Temp(FpName));
    } else {
        baseAddresss = table->GetScopedClass()->GetClassStruct()->GetFieldFrom(
                    identifierName,
                    currentFrame->FindLocalOrFormal(
                            StringSymbol::GetIntern(NameConventions::ThisName)
                        )->GetExp(new Temp(FpName)));
    }
    mainSubtree.reset(new StmWrapper(new Move(baseAddresss, mainSubtree->ToExp())));
}

void IRBuilder::Visit(const AST::PrintLineStatement* node)
{
    node->ExpressionToPrint->AcceptVisitor(this);
    TypeStackVisitor.PopTypeFromStack();
    mainSubtree.reset(new ExpWrapper(new Call(new Name(PrintLnName), new ExpList(mainSubtree->ToExp()))));
}

void IRBuilder::Visit(const AST::WhileStatement* node)
{
    node->Condition->AcceptVisitor(this);
    Label* conditionLabel = Label::GetNextEnumeratedLabel();
    Label* bodyLabel = Label::GetNextEnumeratedLabel();
    Label* exitLabel = Label::GetNextEnumeratedLabel();
    const IStm* condition = mainSubtree->ToConditional(bodyLabel, exitLabel);
    const IStm* conditionPart = new Seq(new LabelStm(conditionLabel), condition);
    node->Body->AcceptVisitor(this);
    const IStm* bodyPart = new Seq(new LabelStm(bodyLabel), new Seq(mainSubtree->ToStm(), new Jump(conditionLabel)));
    mainSubtree.reset(new StmWrapper(new Seq(new Seq(conditionPart, bodyPart), new LabelStm(exitLabel))));
}

void IRBuilder::Visit(const AST::BraceSequenceStatement* node)
{
    node->BracedSequence->AcceptVisitor(this);
}

void IRBuilder::Visit(const AST::IfElseStatement* node)
{
    node->Condition->AcceptVisitor(this);
    TypeStackVisitor.PopTypeFromStack();
    Label* ifBranchLabel = Label::GetNextEnumeratedLabel();
    Label* elseBranchLabel = Label::GetNextEnumeratedLabel();
    Label* exitLabel = Label::GetNextEnumeratedLabel();
    const IStm* condition = mainSubtree->ToConditional(ifBranchLabel, elseBranchLabel);
    node->IfStatement->AcceptVisitor(this);
    const IStm* ifPart = new Seq(new Seq(new LabelStm(ifBranchLabel), mainSubtree->ToStm()), new Jump(exitLabel));
    node->ElseStatement->AcceptVisitor(this);
    const IStm* elsePart = new Seq(new Seq(new LabelStm(elseBranchLabel), mainSubtree->ToStm()), new Jump(exitLabel));
    mainSubtree.reset(new StmWrapper(new Seq(new Seq(condition, new Seq(ifPart, elsePart)), new LabelStm(exitLabel))));
}

void IRBuilder::Visit(const AST::Sequence<const AST::IStatement>* node)
{
    std::vector<const IStm*> statements;
    for(auto const& statement: node->SequenceList) {
        statement->AcceptVisitor(this);
        statements.push_back(mainSubtree->ToStm());
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
    // for "less" case
    Label* trueLabel;
    Label* falseLabel;
    Label* returnLabel;
    IStm* condition;
    Temp* expValue;
    IStm* trueBranch;
    IStm* falseBranch;
    // for all cases
    const IExp* result;
    node->Left->AcceptVisitor(this);
    const IExp* left = mainSubtree->ToExp();
    TypeStackVisitor.PopTypeFromStack();
    node->Right->AcceptVisitor(this);
    const IExp* right = mainSubtree->ToExp();
    TypeStackVisitor.PopTypeFromStack();

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
    case AST::BET_Or:
        result = new Binop(Binop::TB_OR, left, right);
        break;
    case AST::BET_Less:
        trueLabel = Label::GetNextEnumeratedLabel();
        falseLabel = Label::GetNextEnumeratedLabel();
        returnLabel = Label::GetNextEnumeratedLabel();
        condition = new JumpC(JumpC::TJ_LT, left, right, trueLabel, falseLabel);
        expValue = new Temp("expValue");
        trueBranch = new Seq(new Seq(new LabelStm(trueLabel), new Move(expValue, new Const(1))),
                                   new Jump(returnLabel));
        falseBranch = new Seq(new Seq(new LabelStm(falseLabel), new Move(new Temp(*expValue), new Const(1))),
                                    new Jump(returnLabel));
        result = new Eseq(new Seq(new Seq(new Seq(condition, trueBranch), falseBranch), new LabelStm(returnLabel)),
                                        new Mem(new Temp(*expValue)));
        break;
     case AST::BET_Mod:
        result = new Binop(Binop::TB_MOD, left, right);
        break;
    default:
        break;
    }
    mainSubtree.reset(new ExpWrapper(result));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::ArrayMemberExpression* node)
{
    node->BaseExpression->AcceptVisitor(this);
    const IExp* arrayBase = mainSubtree->ToExp();
    node->ElementNumberExpression->AcceptVisitor(this);
    const IExp* elementNumber = mainSubtree->ToExp();
    mainSubtree.reset(new ExpWrapper(ArrayStruct::GetElement(arrayBase, elementNumber)));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::ArrayLengthExpression* node)
{
    node->ArrayExpression->AcceptVisitor(this);
    const IExp* arrayBase = mainSubtree->ToExp();
    mainSubtree.reset(new ExpWrapper(ArrayStruct::GetLength(arrayBase)));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::CallMemberExpression* node)
{
    node->BaseExpression->AcceptVisitor(this);
    const Temp* baseAddress = new Temp(0);
    const IExp* baseExp = new Eseq(new Move(baseAddress, mainSubtree->ToExp()),
                                   new Temp(*baseAddress));
    auto info = TypeStackVisitor.GetTypeFromStack();
    assert(info != nullptr);
    assert(info->GetType() == SymbolTable::VT_UserClass);
    SymbolTable::TypeScopeSwitcher typeSwitcher(*info, table, toPosition(node->Coords()));

    ExpList* arguments = new ExpList(baseExp);
    if( node->ArgumentSequence != nullptr ) {
        auto& list = node->ArgumentSequence->SequenceList;
        for(auto arg = list.begin();
                arg != list.end(); ++arg) {
            (*arg)->AcceptVisitor(this);
            arguments = new ExpList(mainSubtree->ToExp(), arguments);
            TypeStackVisitor.PopTypeFromStack();
        }
    }

    auto classInfo = table->GetClass(*info->GetUserClass());
    const IExp* methodAddress = classInfo->GetClassStruct()->GetVirtualMethodAddress(
                StringSymbol::GetIntern(node->CalledMember->Name), new Temp(*baseAddress));
    mainSubtree.reset(new ExpWrapper(new Call(methodAddress, arguments)));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::Sequence<const AST::IExpression>* node)
{
    ExpList* list = new ExpList();
    for(auto& exp: node->SequenceList) {
        exp->AcceptVisitor(this);
        list = new ExpList(mainSubtree->ToExp(), list);
        TypeStackVisitor.PopTypeFromStack();
    }
    mainSubtree.reset(new ExpWrapper(list));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::ValueExpression* node)
{
    mainSubtree.reset(new ExpWrapper(new Const(node->Value)));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::IdExpression* node)
{
    const ActivationRecords::IAccess* varAccess =
            currentFrame->FindLocalOrFormal(StringSymbol::GetIntern(node->ExpressionId->Name));
    const IExp* varExp = 0;
    if(varAccess != nullptr) {
        varExp = varAccess->GetExp(new Temp(FpName));
    } else {
        assert(table->GetScopedClass() != nullptr);
        varExp = table->GetScopedClass()->GetClassStruct()->GetFieldFrom(
                    StringSymbol::GetIntern(node->ExpressionId->Name),
                    currentFrame->FindLocalOrFormal(
                        StringSymbol::GetIntern(NameConventions::ThisName))->GetExp(new Temp(FpName)));
    }

    TypeStackVisitor.Visit(node);
    mainSubtree.reset(new ExpWrapper(varExp));
}

void IRBuilder::Visit(const AST::ThisExpression* node)
{
    mainSubtree.reset(new ExpWrapper(
                          new Mem(currentFrame->FindLocalOrFormal(StringSymbol::GetIntern(NameConventions::ThisName))->GetExp(
                                      new Temp(FpName)))));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::NewIntArrayExpression* node)
{
    node->NumberOfElements->AcceptVisitor(this);
    TypeStackVisitor.PopTypeFromStack();
    const IExp* numberOfElements = mainSubtree->ToExp();
    ExpList* args = new ExpList(new Binop(Binop::TB_MUL, numberOfElements,
                                          new Const(currentFrame->TypeSize(SymbolTable::VT_Int))));
    mainSubtree.reset(new ExpWrapper(new Call(new Name(NameConventions::MallocName), args )));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::NewObjectExpression* node)
{
    auto classInfo = table->GetClass(node->ObjectId->Name);
    const IExp* allocActions = classInfo->GetClassStruct()->AllocateNew();
    mainSubtree.reset(new ExpWrapper(allocActions));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::NotExpression* node)
{
    node->Expression->AcceptVisitor(this);
    TypeStackVisitor.PopTypeFromStack();
    mainSubtree.reset(new ExpWrapper(new Unaryop(Unaryop::TU_NOT, mainSubtree->ToExp())));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::ContainerExpression* node)
{
    node->Expression->AcceptVisitor(this);
}


}
