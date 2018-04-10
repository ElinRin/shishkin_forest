#include "IInstruction.h";
#include "ISubtreeWrapper.h";
#include "IRNodeTypes.h";

using namespace IR;

namespace CodeGeneration {

    void Muncher::Generation()
    {
        for( auto& stm : stmList ) {
            munchStm( stm.get() );
        }
    }

    void Muncher::munchStm(IStm s) {

        if (Move* move = dynamic_cast<Move*>(s)) {
            munchMove(move->Destination.get(), move->Source.get());
        }

        if (ExpList* exp = dynamic_cast<ExpList*>(s)) {
            munchExpList(exp->Head.get(), exp->Tail.get());
        }

        if (Jump* jump = dynamic_cast<Jump*>(s)) {
            munchJump(jump->LabelToJump);
        }

        if (JumpC* jumpc = dynamic_cast<JumpC*>(s)) {
            munchJumpC(jumpc->ConditionLeftExpression.get(), jumpc->ConditionRightExpression.get(), jumpc->TrueLabel, jumpc->JumpType);
        }

        if (LabelStm* labelstm = dynamic_cast<LabelStm*>(s)) {
            munchLabelStm(labelstm->LabelName);
        }
    }

    void Muncher::munchMove(IExp Destination, IExp Source) {

        if (Mem* mem = dynamic_cast<Mem*>(Destination)) {
            munchMove(mem, Source);
        }
        if (Temp* temp = dynamic_cast<Temp*>(Destination)) {
            munchMove(temp, Source);
        }

    }

    void Muncher::munchExp(IExp exp) {
        //MEM(BINOP(PLUS, e1, CONST(i)))
        if ((Mem* mem = dynamic_cast<Mem*>(exp)) && (Binop* binop = dynamic_cast<Binop*>(mem->Expression.get())) && (binop->Operation == TB_PLUS)
            && (Const* c = dynamic_cast<Const*>(binop->RightExpression.get())) ) {
            munchExp(binop->LeftExpression.get());
            emit("LOAD");
        }
        //MEM(BINOP(PLUS, CONST(i), e1))
        else if ((Mem* mem = dynamic_cast<Mem*>(exp)) && (Binop* binop = dynamic_cast<Binop*>(mem->Expression.get())) && (binop->Operation == TB_PLUS)
                 && (Const* c = dynamic_cast<Const*>(binop->LeftExpression.get())) ) {
            munchExp(binop->RightExpression.get());
            emit("LOAD");
        }
        //MEM(CONST(i))
        else if ((Mem* mem = dynamic_cast<Mem*>(exp)) && Const* c = dynamic_cast<Const*>(Destination.Expression.get())) {
            emit("LOAD");
        }
        //MEM(e1)
        else if (Mem* mem = dynamic_cast<Mem*>(exp)) {
            munchExp(mem->Expression.get());
            emit("LOAD");
        }
        //BINOP(PLUS, e1, CONST(i))
        else if ((Binop* binop = dynamic_cast<Binop*>(exp)) && (binop->Operation == TB_PLUS)
                 && (Const* c = dynamic_cast<Const*>(binop->RightExpression.get())) ) {
            munchExp(binop->LeftExpression.get());
            emit("ADDI");
        }
        //BINOP(PLUS, CONST(i, e1)
        else if ((Binop* binop = dynamic_cast<Binop*>(exp) && (binop->Operation == TB_PLUS)
                 && (Const* c = dynamic_cast<Const*>(binop->LeftExpression.get())) ) {
            munchExp(binop->RightExpression.get());
            emit("ADDI");
        }
        //CONST(i) ⇒ munchExp(e1); emit("ADDI");
        else if (Const* c = dynamic_cast<Const*>(exp)) {
            munchExp(e1); //??????
            emit("ADDI");
        }
        //BINOP(PLUS, e1, CONST(i)) ⇒ munchExp(e1); emit("ADD");
        //TEMP(t) ⇒ {}
    }

    void Muncher::munchMove(Mem Destination, IExp Source) {
        // MOVE(MEM(BINOP(PLUS, e1, CONST(i))), e2)
        if ((Binop* binop = dynamic_cast<Binop*>(Destination.Expression.get())) && (binop->Operation == TB_PLUS)
                               && (Const* c = dynamic_cast<Const*>(binop->RightExpression.get())) ) {
            munchExp(binop->LeftExpression.get());
            munchExp(Source);
            emit("STORE");
        }
        // MOVE(MEM(BINOP(PLUS, CONST(i), e1)), e2)
        else if ((Binop* binop = dynamic_cast<Binop*>(Destination.Expression.get())) && (binop->Operation == TB_PLUS)
                 && (Const* c = dynamic_cast<Const*>(binop->LeftExpression.get())) ) {
            munchExp(binop->RightExpression.get());
            munchExp(src);
            emit("STORE");
        }
        // MOVE(MEM(e1), MEM(e2))
        else if (Mem* mem = dynamic_cast<Mem*>(Source)) {
            munchExp(Destination.Expression.get());
            munchExp(mem->Expression.get());
            emit("MOVEM");
        }
        // MOVE(MEM(e1, e2)
        else {
            munchExp(Destination.Expression.get());
            munchExp(Source);
            emit("STORE");
        }
    }

    void Muncher::munchMove(Temp Destination, IExp Source) {
        // MOVE(TEMP(t1), e)
        munchExp(Sourse);
        emit("ADD");
    }

    void Muncher::munchExpList(Head, Tail) {

    }

    void Muncher::munchJump(Label* LabelToJump) {
        emit();
    }

    void Muncher::munchJumpC(IExp  ConditionLeftExpression, IExp ConditionRightExpression) {
        emit();
    }

    void Muncher::munchLabelStm(Label* LabelName) {
        emit();
    }

}

void Muncher::emit( IInstruction* instruction )
{
    instructionsList.push_back( std::unique_ptr<IInstruction>( instruction ) );
}



