#pragma once
#include "common.h"

#include "IRNodeTypes.h"
#include "Label.h"

namespace AR = ActivationRecords;

namespace IRTranslate {
  
interface ISubtreeWrapper {
    virtual ~ISubtreeWrapper() { }

    virtual const IR::IExp* ToExp() = 0;
    virtual const IR::IStm* ToStm() = 0;
    virtual const IR::IStm* ToConditional(const IR::Label* trueLabel,
                                          const IR::Label* falseLabel) = 0;
};
  
}
