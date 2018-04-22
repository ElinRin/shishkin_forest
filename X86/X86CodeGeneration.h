#include <Instruction.h>
#include <SubtreeWrapper.h>
#include <IRNodeTypes.h>
#include <X86InstructionSet.h>

using namespace IR;

namespace CodeGeneration {

    class Muncher {
    public:
        Muncher(IRTranslate::LinearTree& tree) :
                stmList(tree)
        {}
        const InstructionList CreateInstractionsList();

    private:
        void generation();
        const IRTranslate::LinearTree& stmList;
        InstructionList instructionsList;

        void munchStm(const IStm* s);
        const Temp* munchExp(const IExp* exp);
        
        void munchMove(const IExp* source, const IExp* destination);
        void munchMove(const Mem* source, const IExp* destination);
        void munchMove(const IExp* source, const Mem* destination);
        void munchMove(const IExp* source, const Temp* destination);

        TempList munchExpList(const ExpList* list);
        const Temp* munchMem(const Mem* mem);
        const Temp* munchCall(const Call* call);
        const Temp* munchBinop(const Binop* binop);
        const Temp* munchBinopRegular(const Binop* binop, const std::string& prefix);
        const Temp* munchBinopMul(const Binop* binop);
        const Temp* munchBinopDiv(const Binop* binop);
        void munchJump(const Label* LabelToJump);
        void munchJumpC(const IExp* conditionLeftExpression, const IExp* conditionRightExpression, const Label* trueLabel, JumpC::TJumpType jumpType);
        void munchLabelStm(const Label* LabelName);

        const IInstruction* emit(const IInstruction* nextInstruction);

    };



}

