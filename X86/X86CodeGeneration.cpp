#include <Instruction.h>
#include <SubtreeWrapper.h>
#include <IRNodeTypes.h>
#include <X86CodeGeneration.h>
#include <X86InstructionSet.h>

using namespace IR;

namespace CodeGeneration {

const InstructionList Muncher::CreateInstractionsList()
{
    generation();
    return InstructionList(std::move(instructionsList));
}

void Muncher::generation()
{
    for( auto& stm : stmList ) {
        munchStm( stm.get() );
    }
}

void Muncher::munchStm(const IStm* s) {

    if (const Move* move = dynamic_cast<const Move*>(s)) {
        munchMove(move->Source.get(), move->Destination.get());
    }
    else if (const Jump* jump = dynamic_cast<const Jump*>(s)) {
        munchJump(jump->LabelToJump);
    }
    else if (const JumpC* jumpc = dynamic_cast<const JumpC*>(s)) {
        munchJumpC(jumpc->ConditionLeftExpression.get(), jumpc->ConditionRightExpression.get(), jumpc->TrueLabel, jumpc->JumpType);
    }
    else if (const LabelStm* labelstm = dynamic_cast<const LabelStm*>(s)) {
        munchLabelStm(labelstm->LabelName);
    }
    else if( const Exp* exp = dynamic_cast<const Exp*>(s)) {
        munchExp(exp->expression.get());
    }
    else {
        assert(false);
    }
}

void Muncher::munchMove(const IExp* source, const IExp* destination) {
    if (const Mem* temp = dynamic_cast<const Mem*>(destination)) {
            munchMove(source, temp);
    } else if(const Mem* temp = dynamic_cast<const Mem*>(source)) {
            munchMove(temp, destination);
    } else if(const Temp* temp = dynamic_cast<const Temp*>(destination)){
        munchMove(source, temp);
    } else {
        emit(new X86::RegMove("MOV %0 %1", munchExp(source), munchExp(destination), true));
    }
}

const Temp* Muncher::munchExp(const IExp* exp) {
    //MEM
    if(  const Mem* mem = dynamic_cast<const Mem*>(exp) ) {
        return munchMem(mem);
    }
    //BINOP
    if( const Binop* binop = dynamic_cast<const Binop*>(exp) ) {
        return munchBinop(binop);
    }
    //CONST
    if(const Const* c = dynamic_cast<const Const*>(exp)) {
        Temp* returnedReg = new Temp("Const");
        instructionsList.Registers.push_back(std::unique_ptr<const Temp>(returnedReg));
        emit(new X86::RegMove("MOV %0 " + std::to_string(c->value), c, returnedReg));
        return returnedReg;
    }
    //TEMP
    if(const Temp* c = dynamic_cast<const Temp*>(exp)) {
        return c;
    }
    //CALL
    if(const Call* call = dynamic_cast<const Call*>(exp)) {
        return munchCall(call);
    }
    //NAME
    if(const Name* name = dynamic_cast<const Name*>(exp)) {
        const Temp* result = new Temp("Name");
        instructionsList.Registers.push_back(std::unique_ptr<const Temp>(result));
        const Const* constPtr = nullptr;
        emit(new X86::RegMove("MOV %0" + name->LabelName->GetName(), constPtr, result));
        return result;
    }
    //UNARY
    if(const Unaryop* unary = dynamic_cast<const Unaryop*>(exp)) {
        const Temp* result;
        switch (unary->Operation) {
        case Unaryop::TU_NOT:
            result = new Temp("NOT");
            instructionsList.Registers.push_back(std::unique_ptr<const Temp>(result));
            emit(new X86::RegMove("MOV %0 %1", munchExp(unary->Expression.get()), result, true));
            emit(new X86::CISCOperation("NOT %0", std::move(ConstTempList({result})),
                                        std::move(ConstTempList({result}))));
            return result;
        default:
            assert(false);
            break;
        }
    }
    assert(false);
}

const IInstruction* Muncher::emit(const IInstruction* nextInstruction)
{
    instructionsList.Instructions.push_back( std::unique_ptr<const IInstruction>( nextInstruction ) );
    return nextInstruction;
}

void Muncher::munchMove(const Mem* source, const IExp* destination) {
    // MOVE(MEM(BINOP(...))
    const Binop* binop = dynamic_cast<const Binop*>(source->Expression.get());
    if (binop && (binop->Operation == Binop::TB_PLUS) ) {
        // MOVE(MEM(BINOP(PLUS, e1, CONST(i))), e2)
        if(const Const* c = dynamic_cast<const Const*>(binop->RightExpression.get())) {
            emit(new X86::RegMove("MOV %0 [%1+" + std::to_string(c->value) + "]",
                              munchExp(binop->LeftExpression.get()), munchExp(destination)));
        }
        // MOVE(MEM(BINOP(PLUS, CONST(i), e1)), e2)
        else if(const Const* c = dynamic_cast<const Const*>(binop->LeftExpression.get())) {
            emit(new X86::RegMove("MOV %0 [%1+" + std::to_string(c->value) + "]",
                              munchExp(binop->RightExpression.get()), munchExp(destination)));
        }
        // MOVE(MEM())
        else {
            emit(new X86::RegMove("MOV %0 [%1]",
                              munchExp(binop), munchExp(destination)));
        }
    }
    // MOVE(MEM(e1), e2)
    else if(const Temp* temp = dynamic_cast<const Temp*>(source->Expression.get()) ) {
        emit(new X86::RegMove("MOV %0 %1",
                          temp, munchExp(destination), true));
    } else {
        emit(new X86::RegMove("MOV %0 [%1]",
                          munchExp(source->Expression.get()), munchExp(destination)));
    }
}

void Muncher::munchMove(const IExp* source, const Mem* destination)
{
    // MOVE(MEM(BINOP(...))
    const Binop* binop = dynamic_cast<const Binop*>(destination->Expression.get());
    if (binop && (binop->Operation == Binop::TB_PLUS) ) {
        // MOVE(MEM(BINOP(PLUS, e1, CONST(i))), e2)
        if(const Const* c = dynamic_cast<const Const*>(binop->RightExpression.get())) {
            emit(new X86::RegMove("MOV [%0 + " + std::to_string(c->value) + "] %1",
                                  std::move(ConstTempList({munchExp(binop->LeftExpression.get()), munchExp(source)}))));
        }
        // MOVE(MEM(BINOP(PLUS, CONST(i), e1)), e2)
        else if(const Const* c = dynamic_cast<const Const*>(binop->LeftExpression.get())) {
            emit(new X86::RegMove("MOV [%0 + " + std::to_string(c->value) + "] %1",
                              std::move(ConstTempList({munchExp(binop->RightExpression.get()), munchExp(source)}))));
        }
        // MOVE(MEM())
        else {
            emit(new X86::RegMove("MOV [%0] %1",
                              std::move(ConstTempList({munchExp(binop), munchExp(source)}))));
        }
    }
    // MOVE(MEM(e1), e2)
    else  {
        emit(new X86::RegMove("MOV [%0] %1",
                          std::move(ConstTempList({munchExp(destination->Expression.get()), munchExp(source)}))));
    }
}

void Muncher::munchMove(const IExp* source, const Temp* destination) {
    // MOVE(TEMP(t1), e)
    emit(new X86::RegMove("MOV %0 %1", munchExp(source), destination, true));
}

ConstTempList Muncher::munchExpList(const ExpList* list)
{
    const IExp* head = list->Head.get();
    const IExp* tail = list->Tail.get();
    ConstTempList temps;
    if(const ExpList* subList = dynamic_cast<const ExpList*>(head)) {
        ConstTempList subTemps = munchExpList(subList);
        temps.insert(temps.end(), subTemps.begin(), subTemps.end());
    } else {
        temps.push_back(munchExp(head));
    }
    if(const ExpList* subList = dynamic_cast<const ExpList*>(tail)) {
        ConstTempList subTemps = munchExpList(subList);
        temps.insert(temps.end(), subTemps.begin(), subTemps.end());
    } else if( tail != nullptr ){
        temps.push_back(munchExp(tail));
    }
    return temps;
}

const Temp* Muncher::munchMem(const Mem* mem)
{
    //MEM(BINOP)
    const Binop* binop = dynamic_cast<const Binop*>(mem->Expression.get());
    if( binop && binop->Operation == Binop::TB_PLUS)
    {
        //MEM(BINOP(PLUS, e1, CONST(i)))
        if(const Const* c = dynamic_cast<const Const*>(binop->RightExpression.get()))
        {
            const Temp* left = munchExp(binop->LeftExpression.get());
            Temp* returnedReg = new Temp("MEM(BINOP(PLUS, e1, CONST(i)))");
            instructionsList.Registers.push_back(std::unique_ptr<const Temp>(returnedReg));
            emit(new X86::RegMove("MOV %0 [%1 + " + std::to_string(c->value) + "]",
                                               left, returnedReg));
            return returnedReg;
        }
        //MEM(BINOP(PLUS, CONST(i), e1))
        else if(const Const* c = dynamic_cast<const Const*>(binop->LeftExpression.get()))
        {
            const Temp* right = munchExp(binop->RightExpression.get());
            Temp* returnedReg = new Temp("MEM(BINOP(PLUS, CONST(i), e1))");
            instructionsList.Registers.push_back(std::unique_ptr<const Temp>(returnedReg));
            emit(new X86::RegMove("MOV %0 [%1 + " + std::to_string(c->value) + "]",
                                               right, returnedReg));
            return returnedReg;
        }
        //MEM(e1)
        else {
            Temp* returnedReg = new Temp("MEM(e1)");
            instructionsList.Registers.push_back(std::unique_ptr<const Temp>(returnedReg));
            emit(new X86::RegMove("MOV %0 [%1]", munchExp(binop), returnedReg));
            return returnedReg;
        }
    }

    //MEM(CONST(i))
    else if (const Const* c = dynamic_cast<Const*>(mem->Expression.get())) {
        Temp* returnedReg = new Temp("MEM(Const(i))");
        instructionsList.Registers.push_back(std::unique_ptr<const Temp>(returnedReg));
        emit(new X86::RegMove("MOV %0 [" + std::to_string(c->value) + "]",
                                           c, returnedReg));
        return returnedReg;
    }
    //MEM(e1)
    else {
        const Temp* e1 = munchExp(mem->Expression.get());
        Temp* returnedReg = new Temp("MEM(e1)");
        instructionsList.Registers.push_back(std::unique_ptr<const Temp>(returnedReg));
        emit(new X86::RegMove("MOV %0 [%1]", e1, returnedReg));
        return returnedReg;
    }

}

const Temp*Muncher::munchCall(const Call* call)
{
    const Temp* eax = new Temp("EAX", Coords(), X86::EAX);
    instructionsList.Registers.push_back(std::unique_ptr<const Temp>(eax));
    ConstTempList listArgs(munchExpList(call->Arguments.get()));
    if(const Mem* mem = dynamic_cast<const Mem*>(call->FuncExpression.get())) {
        const Temp* funcAddress = dynamic_cast<const Temp*>(mem->Expression.get());
        assert(funcAddress);
        listArgs.push_back(funcAddress);
        emit(new X86::CISCOperation("CALL [%" + std::to_string(listArgs.size() - 1) +"]", std::move(listArgs),
                                    std::move(ConstTempList())));
        return eax;
    }
    if(const Name* name = dynamic_cast<const Name*>(call->FuncExpression.get())) {
        emit(new X86::CISCOperation("CALL " + name->LabelName->GetName(), std::move(listArgs),
                                    std::move(ConstTempList())));
        return eax;
    }
    assert(false);
}

const Temp* Muncher::munchBinop(const Binop* binop)
{
    switch (binop->Operation) {
    case Binop::TB_PLUS:
        return munchBinopRegular(binop, "ADD ");
    case Binop::TB_MINUS:
        return munchBinopRegular(binop, "SUB ");
    case Binop::TB_OR:
        return munchBinopRegular(binop, "OR ");
    case Binop::TB_AND:
        return munchBinopRegular(binop, "AND ");
    case Binop::TB_MOD:
        return munchBinopDiv(binop);
    case Binop::TB_MUL:
        return munchBinopMul(binop);
    default:
        assert(false);
        break;
    }
}

const Temp* Muncher::munchBinopRegular(const Binop* binop, const std::string& prefix)
{
    Temp* returnReg = new Temp("BINOP(Regular)");
    instructionsList.Registers.push_back(std::unique_ptr<const Temp>(returnReg));
    //BINOP(_, e1, Const)
    if( const Const* left = dynamic_cast<const Const*>(binop->LeftExpression.get()) ) {
        emit(new X86::RegMove("MOV %0 %1", munchExp(binop->RightExpression.get()), returnReg, true));
        emit(new X86::CISCOperation(prefix + "%0 " + std::to_string(left->value),
                                    std::move(ConstTempList({returnReg})), std::move(ConstTempList({returnReg}))));
        return returnReg;
    }
    //BINOP(_, Const, e1)
    else if(const Const* right = dynamic_cast<const Const*>(binop->RightExpression.get())) {
        emit(new X86::RegMove("MOV %0 %1", munchExp(binop->LeftExpression.get()), returnReg, true));
        emit(new X86::CISCOperation(prefix + "%0 " + std::to_string(right->value),
                                    std::move(ConstTempList({returnReg})), std::move(ConstTempList({returnReg}))));
        return returnReg;
    }
    //BINOP(_, e1, e2)
    else {
        emit(new X86::RegMove("MOV %0 %1", munchExp(binop->LeftExpression.get()), returnReg, true));
        emit(new X86::CISCOperation(prefix + "%0 %1",
                                    std::move(ConstTempList({munchExp(binop->RightExpression.get())})),
                                    std::move(ConstTempList({returnReg}))));
        return returnReg;
    }
}

const Temp*Muncher::munchBinopMul(const Binop* binop)
{
    Temp* returnReg = new Temp("BINOP(Regular)");
    instructionsList.Registers.push_back(std::unique_ptr<const Temp>(returnReg));
    //BINOP(MUL, e1, Const)
    if( const Const* left = dynamic_cast<const Const*>(binop->LeftExpression.get()) ) {
        emit(new X86::CISCOperation("IMUL %0 %1 " + std::to_string(left->value),
                                    std::move(ConstTempList({munchExp(binop->RightExpression.get())})),
                                    std::move(ConstTempList({returnReg}))));
        return returnReg;
    }
    //BINOP(MUL, Const, e1)
    else if(const Const* right = dynamic_cast<const Const*>(binop->RightExpression.get())) {
        emit(new X86::CISCOperation("IMUL %0 %1 " + std::to_string(right->value),
                                    std::move(ConstTempList({munchExp(binop->LeftExpression.get())})),
                                    std::move(ConstTempList({returnReg}))));
        return returnReg;
    }
    //BINOP(MUL, e1, e2)
    else {
        emit(new X86::RegMove("MOV %0 %1", munchExp(binop->LeftExpression.get()), returnReg, true));
        emit(new X86::CISCOperation("IMUL %0 %1",
                                    std::move(ConstTempList({munchExp(binop->RightExpression.get())})),
                                    std::move(ConstTempList({returnReg}))));
        return returnReg;
    }
}

const Temp* Muncher::munchBinopDiv(const Binop* binop)
{
    const Temp* eax = new Temp("EAX", Coords(), X86::EAX);
    instructionsList.Registers.push_back(std::unique_ptr<const Temp>(eax));
    const Temp* edx = new Temp("EDX", Coords(), X86::EDX);
    instructionsList.Registers.push_back(std::unique_ptr<const Temp>(edx));
    emit(new X86::RegMove("MOV %0 %1", munchExp(binop->LeftExpression.get()), eax, true));
    emit(new X86::CISCOperation("IDIV %1",
                                std::move(ConstTempList({munchExp(binop->RightExpression.get()), eax})),
                                std::move(ConstTempList({edx}))));
    Temp* returnReg = new Temp("BINOP(Regular)");
    instructionsList.Registers.push_back(std::unique_ptr<const Temp>(returnReg));
    emit(new X86::RegMove("MOV %0 %1", edx, returnReg, true));
    return returnReg;
}


void Muncher::munchJump(const Label* LabelToJump) {
    emit(new X86::CISCOperation("JMP %l", std::move(ConstTempList()),
                                std::move(ConstTempList()), std::move(LabelList({LabelToJump}))));
}

void Muncher::munchJumpC(const IExp* conditionLeftExpression, const IExp* conditionRightExpression,
                         const Label* trueLabel,
                         JumpC::TJumpType jumpType)
{
    if(const Const* c = dynamic_cast<const Const*>(conditionLeftExpression)) {
        const Temp* e = munchExp(conditionRightExpression);
        emit(new X86::CISCOperation("CMP %0 " + std::to_string(c->value), std::move(ConstTempList({e})), std::move(ConstTempList())));
    }
    else if(const Const* c = dynamic_cast<const Const*>(conditionRightExpression)) {
        const Temp* e = munchExp(conditionLeftExpression);
        emit(new X86::CISCOperation("CMP %0 " + std::to_string(c->value), std::move(ConstTempList({e})), std::move(ConstTempList())));
    }
    else {
        const Temp* e1 = munchExp(conditionLeftExpression);
        const Temp* e2 = munchExp(conditionRightExpression);
        emit(new X86::CISCOperation("CMP %0 %1", std::move(ConstTempList({e1, e2})), std::move(ConstTempList())));
    }
    switch (jumpType) {
    case JumpC::TJ_EQ:
        emit(new X86::CISCOperation("JE %l", std::move(ConstTempList()), std::move(ConstTempList()), std::move(LabelList({trueLabel}))));
        break;
    case JumpC::TJ_LT:
        emit(new X86::CISCOperation("JL %l", std::move(ConstTempList()), std::move(ConstTempList()), std::move(LabelList({trueLabel}))));
        break;
    case JumpC::TJ_NEQ:
        emit(new X86::CISCOperation("JNE %l", std::move(ConstTempList()), std::move(ConstTempList()), std::move(LabelList({trueLabel}))));
        break;
    default:
        break;
    }
}

void Muncher::munchLabelStm(const Label* labelName) {
    emit(new LabelInstruction(labelName));
}

}

