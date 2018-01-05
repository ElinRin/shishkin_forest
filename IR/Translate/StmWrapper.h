#pragma once

#include <memory>
#include <assert.h>
#include "SubtreeWrapper.h"

namespace IRTranslate {

class StmWrapper : public ISubtreeWrapper
{
public:
    StmWrapper(const IR::IStm* statement) : statement(statement) {}

    virtual const IR::IExp* ToExp() override { assert(false); }
    virtual const IR::IStm* ToStm() override { return statement.release(); }
    virtual const IR::IStm* ToConditional(const IR::Label* trueLabel,
                                           const IR::Label* falseLabel) override { assert(false); }
private:
    std::unique_ptr<const IR::IStm> statement;
};

}
