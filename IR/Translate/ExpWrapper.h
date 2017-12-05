#pragma once

#include "SubtreeWrapper.h"

namespace IRTranslate {
class ExpWrapper : public ISubtreeWrapper
{
public:
    IRTranslate(const IR::IExp expression) : expression(expression) {}

    virtual const IR::IExp* ToExp() const override { return expression; }
    virtual const IR::IStm* ToStm() const override;
    virtual const IR::IStm* ToConditional(const AR::Label trueLabel, const AR::Label falseLabel) const override;

private:
    const IR::IExp* expression;
};

}
