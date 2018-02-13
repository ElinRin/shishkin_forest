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

    void CreateGraph(const IRForest& forest);

    virtual void Visit(const IR::Unaryop* node) override;
    virtual void Visit(const IR::Binop* node) override;
    virtual void Visit(const IR::Call* node) override;
    virtual void Visit(const IR::Const* node) override;
    virtual void Visit(const IR::Eseq* node) override;
    virtual void Visit(const IR::Mem* node) override;
    virtual void Visit(const IR::Name* node) override;
    virtual void Visit(const IR::Temp* node) override;
    virtual void Visit(const IR::Exp* node) override;
    virtual void Visit(const IR::Jump* node) override;
    virtual void Visit(const IR::JumpC* node) override;
    virtual void Visit(const IR::LabelStm* node) override;
    virtual void Visit(const IR::Move* node) override;
    virtual void Visit(const IR::Seq* node) override;
    virtual void Visit(const IR::ExpList* node) override;
    virtual void Visit(const IR::StmList* node) override;

    virtual void Visit(const StmWrapper* node) override;
    virtual void Visit(const ExpWrapper* node) override;

private:
    std::string format(IR::Unaryop::TUnaryop op);
    std::string format(IR::Binop::TBinop op);
    std::string format(const IR::Temp *temp);
    std::string format(IR::JumpC::TJumpType type);
    std::string format(const IR::Coords& coords);
};

}
