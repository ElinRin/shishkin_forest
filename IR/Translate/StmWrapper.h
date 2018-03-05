#pragma once

#include <memory>
#include <assert.h>
#include "SubtreeWrapper.h"

namespace IRTranslate {

class StmWrapper : public ISubtreeWrapper
{
public:
    StmWrapper(IR::IStm* statement) : statement(statement) {}

    virtual IR::IExp* ToExp() override { assert(false); }
    virtual IR::IStm* ToStm() override { return statement.release(); }
    virtual IR::IStm* ToConditional(IR::JumpC::TJumpType, const IR::Label* trueLabel) override { assert(false); }

    IR::IStm* Stm() { return statement.get(); }

    ACCEPT_IR_VISITOR

private:
    std::unique_ptr<IR::IStm> statement;
};

}
