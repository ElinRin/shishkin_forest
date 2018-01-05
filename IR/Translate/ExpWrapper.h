#pragma once

#include "SubtreeWrapper.h"

namespace IRTranslate {
class ExpWrapper : public ISubtreeWrapper
{
public:
    ExpWrapper(const IR::IExp* expression) : expression(expression) {}

    virtual const IR::IExp* ToExp() override { return expression.release(); }
    virtual const IR::IStm* ToStm() override;
    virtual const IR::IStm* ToConditional(const IR::Label* trueLabel, const IR::Label* falseLabel)override;

private:
    std::unique_ptr<const IR::IExp> expression;
};

}
