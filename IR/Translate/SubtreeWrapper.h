#pragma once
#include "common.h"

#include "IRNodeTypes.h"

namespace AR = ActivationRecords;

namespace IRTranslate {
  
interface ISubtreeWrapper {
    virtual ~ISubtreeWrapper() { }

    virtual const IR::IExp* ToExp() const = 0;
    virtual const IR::IStm* ToStm() const = 0;
    virtual const IR::IStm* ToConditional(const AR::Label trueLabel,
                                          const AR::Label falseLabel) const = 0;
};
  
}
