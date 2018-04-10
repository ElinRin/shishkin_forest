#include "IInstruction.h";
#include "ISubtreeWrapper.h";
#include "IRNodeTypes.h";

using namespace IR;

namespace CodeGeneration {

    class Muncher {
    private:
        const std::vector<std::unique_ptr<IStm>> stmList;
        std::vector< std::unique_ptr<IInstruction> > instructionsList;

        void munchStm(IStm s);
        void munchExp(IExp exp);
        
        void munchMove(IExp Destination, IExp Source);
        void munchMove(Mem Destination, IExp Source);
        void munchMove(Temp Destination, IExp Source);

        void munchExpList(Head, Tail);
        void munchJump(Label* LabelToJump);
        void munchJumpC(IExp  ConditionLeftExpression, IExp ConditionRightExpression);
        void munchLabelStm(Label* LabelName);
    public:
        Muncher(std::vector<std::unique_ptr<IStm>> statments) :
                stmList(statments) {}
        void Generation();
        const std::vector< std::unique_ptr<IInstruction> >& GetInstractionsList() const {
            return instractionsList;
        }

    };



}

