#pragma once

#include <memory>
#include <assert.h>
#include "SubtreeWrapper.h"

namespace IRTranslate {

class StmWrapper : public ISubtreeWrapper
{
public:
    StmWrapper(const IR::IStm* statement);

    virtual const IR::IExp* ToExp() const override { assert(false); }
    virtual const IR::IStm * ToStm() const override { return statement; }
    virtual const IR::IStm * ToConditional(const AR::Label trueLabel,
                                           const AR::Label falseLabel) const override { assert(false); }
private:
    std::unique_ptr<const IR::IStm> statement;
};

}
