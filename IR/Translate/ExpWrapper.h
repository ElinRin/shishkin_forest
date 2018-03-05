#pragma once

#include "SubtreeWrapper.h"

namespace IRTranslate {
class ExpWrapper : public ISubtreeWrapper
{
public:
    ExpWrapper(IR::IExp* expression) : expression(expression) {}

    virtual IR::IExp* ToExp() override { return expression.release(); }
    virtual IR::IStm* ToStm() override;
    virtual IR::IStm* ToConditional(IR::JumpC::TJumpType type,
                                          const IR::Label* trueLabel) override;

    IR::IExp* Exp() { return expression.get(); }

    ACCEPT_IR_VISITOR
private:
    std::unique_ptr<IR::IExp> expression;
};

}
