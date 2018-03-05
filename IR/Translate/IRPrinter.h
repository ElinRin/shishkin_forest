#pragma once
#include "common.h"

#include <fstream>

#include "Framework/DotPrint.h"
#include "IRVisitor.h"
#include "SubtreeWrapper.h"

namespace IRTranslate {

class IRPrinter : public IR::IIRVisitor, private DotPrint {
public:
    IRPrinter(std::string filename) :
        DotPrint(filename)
    {
    }

    void CreateGraph(IRForest& forest);

    virtual void Visit(IR::Unaryop* node) override;
    virtual void Visit(IR::Binop* node) override;
    virtual void Visit(IR::Call* node) override;
    virtual void Visit(IR::Const* node) override;
    virtual void Visit(IR::Eseq* node) override;
    virtual void Visit(IR::Mem* node) override;
    virtual void Visit(IR::Name* node) override;
    virtual void Visit(IR::Temp* node) override;
    virtual void Visit(IR::Exp* node) override;
    virtual void Visit(IR::Jump* node) override;
    virtual void Visit(IR::JumpC* node) override;
    virtual void Visit(IR::LabelStm* node) override;
    virtual void Visit(IR::Move* node) override;
    virtual void Visit(IR::Seq* node) override;
    virtual void Visit(IR::ExpList* node) override;

    virtual void Visit(StmWrapper* node) override;
    virtual void Visit(ExpWrapper* node) override;

private:
    std::string format(IR::Unaryop::TUnaryop op);
    std::string format(IR::Binop::TBinop op);
    std::string format(const IR::Temp *temp);
    std::string format(IR::JumpC::TJumpType type);
    std::string format(const IR::Coords& coords);
};

}
