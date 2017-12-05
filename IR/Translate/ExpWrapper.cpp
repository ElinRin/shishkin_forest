#include "ExpWrapper.h"
#include "Exp.h"
#include "JumpC.h"

namespace IRTranslate {

const IR::IStm* ExpWrapper::ToStm() const
{
    return new IR::Exp(expression);
}

const IR::IStm* ExpWrapper::ToConditional(const AR::Label trueLabel,
                                          const AR::Label falseLabel) const
{
    return new IR::JumpC(expression, trueLabel, falseLabel);
}

}
