#pragma once

#include <IRVisitor.h>
#include <vector>

using namespace IR;

namespace IRTranslate {

class Linerizer : public IIRVisitor {
public:
    Linerizer() : statements(nullptr) {}

    void Linerize(ISubtreeWrapper* wrapper, std::vector<std::unique_ptr<IStm>>& stms);

    // Expressions
    virtual void Visit(IR::Unaryop* node) override { assert(false); }
    virtual void Visit(IR::Binop* node) override { assert(false); }
    virtual void Visit(IR::Call* node) override { assert(false); }
    virtual void Visit(IR::Const* node) override { assert(false); }
    virtual void Visit(IR::Eseq* node) override { assert(false); }
    virtual void Visit(IR::Mem* node) override { assert(false); }
    virtual void Visit(IR::Name* node) override { assert(false); }
    virtual void Visit(IR::Temp* node) override { assert(false); }
    virtual void Visit(IR::ExpList* node) override { assert(false); }
    // Statements
    virtual void Visit(IR::Exp* node) override { addToStatements(node); }
    virtual void Visit(IR::Jump* node) override { addToStatements(node); }
    virtual void Visit(IR::JumpC* node) override { addToStatements(node); }
    virtual void Visit(IR::LabelStm* node) override { addToStatements(node); }
    virtual void Visit(IR::Move* node) override { addToStatements(node); }
    virtual void Visit(IR::Seq* node) override;

    virtual void Visit(StmWrapper* node) override;
    virtual void Visit(ExpWrapper* node) override;

private:
    bool isPreviousDetached;
    std::vector<std::unique_ptr<IStm>>* statements;

    void addToStatements(IStm* stm);
};

}
