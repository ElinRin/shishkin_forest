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
    IStm* stm = nullptr;
    if( node->Statements != nullptr ) {
        node->Statements->AcceptVisitor(this);
        stm = new StmList(mainSubtree->ToStm(), nullptr, toPosition(node->Coords()));
    }
    node->StatementToReturn->AcceptVisitor(this);
    if(stm != nullptr) {
        stm = new StmList(stm, mainSubtree->ToStm(), toPosition(node->Coords()));
    } else {
        stm = mainSubtree->ToStm();
    }
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
    IExp* returnAddress = currentFrame->ReturnAddress()->GetExp(new Temp(FpName, toPosition(node->Coords())), toPosition(node->Coords()));
    mainSubtree.reset(new StmWrapper(new Move(returnAddress, mainSubtree->ToExp(), toPosition(node->Coords()))));
}

void IRBuilder::Visit(const AST::AssignArrayElementStatement* node)
{
    node->ElementNumberExpression->AcceptVisitor(this);
    TypeStackVisitor.PopTypeFromStack();
    std::unique_ptr<ISubtreeWrapper> elementNumberExp(mainSubtree.release());
    node->ExpressionToAssign->AcceptVisitor(this);
    auto identifierName = StringSymbol::GetIntern(node->Identifier->Name);
    const AR::IAccess* access = currentFrame->FindLocalOrFormal(identifierName);
    IExp* baseAddresss = nullptr;
    if(access != nullptr) {
        baseAddresss = access->GetExp(new Temp(FpName, toPosition(node->Coords())), toPosition(node->Coords()));
    } else {
        baseAddresss = table->GetScopedClass()->GetClassStruct()->GetFieldFrom(
                    identifierName,
                    currentFrame->FindLocalOrFormal(
                            StringSymbol::GetIntern(NameConventions::ThisName)
                        )->GetExp(new Temp(FpName, toPosition(node->Coords())), toPosition(node->Coords())), toPosition(node->Coords()));
    }
    IExp* address = new Mem(new Binop(Binop::TB_PLUS, baseAddresss,
                                      new Binop(Binop::TB_MUL,
                                                new Const(currentFrame->TypeSize(currentFrame->WordType().GetType()),
                                                          toPosition(node->Coords())),
                                                elementNumberExp->ToExp(), toPosition(node->Coords())),
                                      toPosition(node->Coords())),
                            toPosition(node->Coords()));
    mainSubtree.reset(new StmWrapper(new Move(address, mainSubtree->ToExp(), toPosition(node->Coords()))));
}

void IRBuilder::Visit(const AST::AssignStatement *node)
{
    node->ExpressionToAssign->AcceptVisitor(this);
    auto identifierName = StringSymbol::GetIntern(node->Identifier->Name);
    const AR::IAccess* access = currentFrame->FindLocalOrFormal(identifierName);
    IExp* baseAddresss = nullptr;
    //CP
    if(access != nullptr) {
        baseAddresss = access->GetExp(new Temp(FpName, toPosition(node->Coords())), toPosition(node->Coords()));
    } else {
        baseAddresss = table->GetScopedClass()->GetClassStruct()->GetFieldFrom(
                    identifierName,
                    currentFrame->FindLocalOrFormal(
                            StringSymbol::GetIntern(NameConventions::ThisName)
                        )->GetExp(new Temp(FpName, toPosition(node->Coords())), toPosition(node->Coords())), toPosition(node->Coords()));
    }
    mainSubtree.reset(new StmWrapper(new Move(baseAddresss, mainSubtree->ToExp(), toPosition(node->Coords()))));
}

void IRBuilder::Visit(const AST::PrintLineStatement* node)
{
    node->ExpressionToPrint->AcceptVisitor(this);
    TypeStackVisitor.PopTypeFromStack();
    mainSubtree.reset(new ExpWrapper(new Call(new Name(PrintLnName, toPosition(node->Coords())),
                                              new ExpList(mainSubtree->ToExp(), nullptr, toPosition(node->Coords())),
                                              toPosition(node->Coords()))));
}

void IRBuilder::Visit(const AST::WhileStatement* node)
{
    node->Condition->AcceptVisitor(this);
    Label* conditionLabel = Label::GetNextEnumeratedLabel();
    Label* exitLabel = Label::GetNextEnumeratedLabel();
    IStm* condition = mainSubtree->ToConditional(IR::JumpC::TJ_NEQ, exitLabel);
    IStm* conditionPart = new Seq(new LabelStm(conditionLabel, toPosition(node->Coords())),
                                        condition, toPosition(node->Coords()));
    node->Body->AcceptVisitor(this);
    IStm* bodyPart = new Seq(mainSubtree->ToStm(), new Jump(conditionLabel, toPosition(node->Coords())),
                                   toPosition(node->Coords()));
    mainSubtree.reset(new StmWrapper(new Seq(new Seq(conditionPart, bodyPart, toPosition(node->Coords())),
                                             new LabelStm(exitLabel, toPosition(node->Coords())),
                                             toPosition(node->Coords()))));
}

void IRBuilder::Visit(const AST::BraceSequenceStatement* node)
{
    node->BracedSequence->AcceptVisitor(this);
}

void IRBuilder::Visit(const AST::IfElseStatement* node)
{
    node->Condition->AcceptVisitor(this);
    TypeStackVisitor.PopTypeFromStack();
    Label* elseBranchLabel = Label::GetNextEnumeratedLabel();
    Label* exitLabel = Label::GetNextEnumeratedLabel();
    IStm* condition = mainSubtree->ToConditional(IR::JumpC::TJ_NEQ, elseBranchLabel);
    node->IfStatement->AcceptVisitor(this);
    IStm* ifPart = new Seq(mainSubtree->ToStm(),
                                 new Jump(exitLabel, toPosition(node->Coords())),
                                 toPosition(node->Coords()));
    node->ElseStatement->AcceptVisitor(this);
    IStm* elsePart = new Seq(new Seq(new LabelStm(elseBranchLabel, toPosition(node->Coords())),
                                           mainSubtree->ToStm(), toPosition(node->Coords())),
                                   new Jump(exitLabel, toPosition(node->Coords())),
                                   toPosition(node->Coords()));
    mainSubtree.reset(new StmWrapper(new Seq(new Seq(condition, new Seq(ifPart, elsePart, toPosition(node->Coords())),
                                                     toPosition(node->Coords())), new LabelStm(exitLabel, toPosition(node->Coords())),
                                             toPosition(node->Coords()))));
}

void IRBuilder::Visit(const AST::Sequence<const AST::IStatement>* node)
{
    std::vector<IStm*> statements;
    for(auto& statement: node->SequenceList) {
        statement->AcceptVisitor(this);
        statements.push_back(mainSubtree->ToStm());
    }
    assert(statements.size() > 0);
    if(statements.size() == 1) {
        mainSubtree.reset(new StmWrapper(statements[0]));
    } else {
        IStm* seq = new Seq(statements[0], statements[1], toPosition(node->Coords()));
        for(int i = 2; i < statements.size(); ++i) {
            seq = new Seq(seq, statements[i], toPosition(node->Coords()));
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
    Label* continueLabel;
    IStm* condition;
    Temp* expValue;
    IStm* trueBranch;
    IStm* falseBranch;
    // for all cases
    IExp* result;
    node->Left->AcceptVisitor(this);
    IExp* left = mainSubtree->ToExp();
    TypeStackVisitor.PopTypeFromStack();
    node->Right->AcceptVisitor(this);
    IExp* right = mainSubtree->ToExp();
    TypeStackVisitor.PopTypeFromStack();

    switch (node->Type) {
    case AST::BET_Minus:
        result = new Binop(Binop::TB_MINUS, left, right, toPosition(node->Coords()));
        break;
    case AST::BET_Mult:
        result = new Binop(Binop::TB_MUL, left, right, toPosition(node->Coords()));
        break;
    case AST::BET_Plus:
        result = new Binop(Binop::TB_PLUS, left, right, toPosition(node->Coords()));
        break;
    case AST::BET_Less:
        trueLabel = Label::GetNextEnumeratedLabel();
        falseLabel = Label::GetNextEnumeratedLabel();
        returnLabel = Label::GetNextEnumeratedLabel();
        condition = new JumpC(JumpC::TJ_LT, left, right, trueLabel,
                              toPosition(node->Coords()));
        expValue = new Temp("expValue", toPosition(node->Coords()));
        trueBranch = new Seq(new Seq(new LabelStm(trueLabel, toPosition(node->Coords())),
                                     new Move(expValue, new Const(1, toPosition(node->Coords())), toPosition(node->Coords())),
                                     toPosition(node->Coords())),
                                   new Jump(returnLabel, toPosition(node->Coords())), toPosition(node->Coords()));
        falseBranch = new Seq(new Seq(new LabelStm(falseLabel, toPosition(node->Coords())),
                                      new Move(new Temp(*expValue),
                                               new Const(0, toPosition(node->Coords())),
                                               toPosition(node->Coords())), toPosition(node->Coords())),
                                    new Jump(returnLabel, toPosition(node->Coords())),
                              toPosition(node->Coords()));
        result = new Eseq(new Seq(new Seq(new Seq(condition, trueBranch, toPosition(node->Coords())),
                                          falseBranch, toPosition(node->Coords())),
                                  new LabelStm(returnLabel, toPosition(node->Coords())), toPosition(node->Coords())),
                           new Mem(new Temp(*expValue), toPosition(node->Coords())), toPosition(node->Coords()));
        break;
     case AST::BET_Mod:
        result = new Binop(Binop::TB_MOD, left, right, toPosition(node->Coords()));
        break;
    case AST::BET_And:
        trueLabel = Label::GetNextEnumeratedLabel();
        falseLabel = Label::GetNextEnumeratedLabel();
        returnLabel = Label::GetNextEnumeratedLabel();
        expValue = new Temp("expValue", toPosition(node->Coords()));
        condition = new JumpC(JumpC::TJ_NEQ, left, new Const(1, toPosition(node->Coords())), falseLabel,toPosition(node->Coords()));
        trueBranch = new Seq(new JumpC(JumpC::TJ_NEQ, right, new Const(1, toPosition(node->Coords())), falseLabel,toPosition(node->Coords())),
                                        new Seq(
                                            new Move(expValue, new Const(1, toPosition(node->Coords())), toPosition(node->Coords())),
                                            new Jump(returnLabel, toPosition(node->Coords())),
                                            toPosition(node->Coords())
                                    ),
                                    toPosition(node->Coords())
                             );
        falseBranch = new Seq(
                        new LabelStm(falseLabel, toPosition(node->Coords())),
                        new Seq(
                            new Move(new Temp(*expValue), new Const(0, toPosition(node->Coords())), toPosition(node->Coords())),
                            new Jump(returnLabel, toPosition(node->Coords())),
                            toPosition(node->Coords())
                        ),
                        toPosition(node->Coords())
                    );
        result = new Eseq(
                    new Seq(
                        new Seq(
                            new Seq(
                                condition,
                                trueBranch,
                                toPosition(node->Coords())
                            ),
                            falseBranch,
                            toPosition(node->Coords())
                        ),
                        new LabelStm(returnLabel, toPosition(node->Coords()))),
                        new Temp(*expValue),
                        toPosition(node->Coords())
                    );
        break;
    case AST::BET_Or:
        trueLabel = Label::GetNextEnumeratedLabel();
        falseLabel = Label::GetNextEnumeratedLabel();
        returnLabel = Label::GetNextEnumeratedLabel();
        expValue = new Temp("expValue", toPosition(node->Coords()));
        condition = new JumpC(JumpC::TJ_EQ, left, new Const(1, toPosition(node->Coords())), trueLabel, toPosition(node->Coords()));
        trueBranch = new Seq(new JumpC(JumpC::TJ_NEQ, right, new Const(1, toPosition(node->Coords())), falseLabel, toPosition(node->Coords())),
                            new Seq(
                                        new LabelStm(trueLabel, toPosition(node->Coords())),
                                        new Seq(
                                            new Move(expValue, new Const(1, toPosition(node->Coords())), toPosition(node->Coords())),
                                            new Jump(returnLabel, toPosition(node->Coords())),
                                            toPosition(node->Coords())
                                        ),
                                        toPosition(node->Coords())
                                    ),
                                    toPosition(node->Coords())
                             );
        falseBranch = new Seq(
                        new LabelStm(falseLabel, toPosition(node->Coords())),
                        new Seq(
                            new Move(new Temp(*expValue), new Const(0, toPosition(node->Coords())), toPosition(node->Coords())),
                            new Jump(returnLabel, toPosition(node->Coords())),
                            toPosition(node->Coords())
                        ),
                        toPosition(node->Coords())
                    );
        result = new Eseq(
                    new Seq(
                        new Seq(
                            new Seq(
                                condition,
                                trueBranch,
                                toPosition(node->Coords())
                            ),
                            falseBranch,
                            toPosition(node->Coords())
                        ),
                        new LabelStm(returnLabel, toPosition(node->Coords())),
                        toPosition(node->Coords())
                        ),
                     new Temp(*expValue),
                     toPosition(node->Coords())
                 );
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
    IExp* arrayBase = mainSubtree->ToExp();
    node->ElementNumberExpression->AcceptVisitor(this);
    IExp* elementNumber = mainSubtree->ToExp();
    mainSubtree.reset(new ExpWrapper(ArrayStruct::GetElement(arrayBase, elementNumber, toPosition(node->Coords()))));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::ArrayLengthExpression* node)
{
    node->ArrayExpression->AcceptVisitor(this);
    IExp* arrayBase = mainSubtree->ToExp();
    mainSubtree.reset(new ExpWrapper(ArrayStruct::GetLength(arrayBase, toPosition(node->Coords()))));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::CallMemberExpression* node)
{
    node->BaseExpression->AcceptVisitor(this);
    Temp* baseAddress = new Temp(0, toPosition(node->Coords()));
    IExp* baseExp = new Eseq(new Move(baseAddress, mainSubtree->ToExp(), toPosition(node->Coords())),
                                   new Temp(*baseAddress), toPosition(node->Coords()));
    auto info = TypeStackVisitor.GetTypeFromStack();
    assert(info != nullptr);
    assert(info->GetType() == SymbolTable::VT_UserClass);
    SymbolTable::TypeScopeSwitcher typeSwitcher(*info, table, toPosition(node->Coords()));

    ExpList* arguments = new ExpList(baseExp, nullptr, toPosition(node->Coords()));
    if( node->ArgumentSequence != nullptr ) {
        auto& list = node->ArgumentSequence->SequenceList;
        for(auto arg = list.begin();
                arg != list.end(); ++arg) {
            (*arg)->AcceptVisitor(this);
            arguments = new ExpList(mainSubtree->ToExp(), arguments, toPosition(node->Coords()));
            TypeStackVisitor.PopTypeFromStack();
        }
    }

    auto classInfo = table->GetClass(*info->GetUserClass());
    IExp* methodAddress = classInfo->GetClassStruct()->GetVirtualMethodAddress(
                StringSymbol::GetIntern(node->CalledMember->Name), new Temp(*baseAddress), toPosition(node->Coords()));
    mainSubtree.reset(new ExpWrapper(new Call(methodAddress, arguments, toPosition(node->Coords()))));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::Sequence<const AST::IExpression>* node)
{
    ExpList* list = new ExpList(nullptr, nullptr, toPosition(node->Coords()));
    for(auto& exp: node->SequenceList) {
        exp->AcceptVisitor(this);
        list = new ExpList(mainSubtree->ToExp(), list, toPosition(node->Coords()));
        TypeStackVisitor.PopTypeFromStack();
    }
    mainSubtree.reset(new ExpWrapper(list));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::ValueExpression* node)
{
    mainSubtree.reset(new ExpWrapper(new Const(node->Value, toPosition(node->Coords()))));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::IdExpression* node)
{
    const ActivationRecords::IAccess* varAccess =
            currentFrame->FindLocalOrFormal(StringSymbol::GetIntern(node->ExpressionId->Name));
    IExp* varExp = 0;
    if(varAccess != nullptr) {
        varExp = varAccess->GetExp(new Temp(FpName, toPosition(node->Coords())), toPosition(node->Coords()));
    } else {
        assert(table->GetScopedClass() != nullptr);
        varExp = table->GetScopedClass()->GetClassStruct()->GetFieldFrom(
                    StringSymbol::GetIntern(node->ExpressionId->Name),
                    currentFrame->FindLocalOrFormal(
                        StringSymbol::GetIntern(NameConventions::ThisName))->GetExp(new Temp(FpName, toPosition(node->Coords())), toPosition(node->Coords())),
                    toPosition(node->Coords()));
    }

    TypeStackVisitor.Visit(node);
    mainSubtree.reset(new ExpWrapper(varExp));
}

void IRBuilder::Visit(const AST::ThisExpression* node)
{
    mainSubtree.reset(new ExpWrapper(
                          new Mem(currentFrame->FindLocalOrFormal(StringSymbol::GetIntern(NameConventions::ThisName))->GetExp(
                                      new Temp(FpName, toPosition(node->Coords())), toPosition(node->Coords())),
                                  toPosition(node->Coords()))));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::NewIntArrayExpression* node)
{
    node->NumberOfElements->AcceptVisitor(this);
    TypeStackVisitor.PopTypeFromStack();
    IExp* numberOfElements = mainSubtree->ToExp();
    ExpList* args = new ExpList(new Binop(Binop::TB_MUL, numberOfElements,
                                          new Const(currentFrame->TypeSize(SymbolTable::VT_Int), toPosition(node->Coords())), toPosition(node->Coords())));
    mainSubtree.reset(new ExpWrapper(new Call(new Name(NameConventions::MallocName, toPosition(node->Coords())), args, toPosition(node->Coords()) )));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::NewObjectExpression* node)
{
    auto classInfo = table->GetClass(node->ObjectId->Name);
    IExp* allocActions = classInfo->GetClassStruct()->AllocateNew(toPosition(node->Coords()));
    mainSubtree.reset(new ExpWrapper(allocActions));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::NotExpression* node)
{
    node->Expression->AcceptVisitor(this);
    TypeStackVisitor.PopTypeFromStack();
    mainSubtree.reset(new ExpWrapper(new Unaryop(Unaryop::TU_NOT, mainSubtree->ToExp(), toPosition(node->Coords()))));
    TypeStackVisitor.Visit(node);
}

void IRBuilder::Visit(const AST::ContainerExpression* node)
{
    node->Expression->AcceptVisitor(this);
}


}
