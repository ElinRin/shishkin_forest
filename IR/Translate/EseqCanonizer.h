#pragma once

#include <IRVisitor.h>

using namespace IR;

namespace IRTranslate {

class EseqCanonizer : public IIRVisitor {
public:
    EseqCanonizer() : lastEseq(new Eseq(nullptr, nullptr)) {  }

    StmWrapper* Canonize(ISubtreeWrapper* wrapper);

    virtual void Visit(Unaryop* node) override;
    virtual void Visit(Binop* node) override;
    virtual void Visit(Call* node) override;
    virtual void Visit(Const* node) override { return lastEseq->Expression.reset(node); }
    virtual void Visit(Eseq* node) override;
    virtual void Visit(Mem* node) override;
    virtual void Visit(Name* node) override;
    virtual void Visit(Temp* node) override;
    virtual void Visit(Exp* node) override;
    virtual void Visit(Jump* node) override;
    virtual void Visit(JumpC* node) override;
    virtual void Visit(LabelStm* node) override;
    virtual void Visit(Move* node) override;
    virtual void Visit(Seq* node) override;
    virtual void Visit(ExpList* node) override;
    virtual void Visit(StmWrapper* node) override;
    virtual void Visit(ExpWrapper* node) override;

private:
    std::unique_ptr<Eseq> lastEseq;
    IStm* reorder(std::unique_ptr<IExp>& exp);
    IStm* addSeqIfRequired(IStm* stm);
    void decomposeEseq();
};

}

