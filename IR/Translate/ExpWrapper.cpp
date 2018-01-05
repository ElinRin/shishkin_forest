#include "ExpWrapper.h"

#include <string>

#include "Exp.h"
#include "JumpC.h"

namespace IRTranslate {

const IR::IStm* ExpWrapper::ToStm()
{
    return new IR::Exp(expression.release());
}

const IR::IStm* ExpWrapper::ToConditional(const IR::Label* trueLabel,
                                          const IR::Label* falseLabel)
{
    std::string name = "true";
    return new IR::JumpC(IR::JumpC::TJ_EQ, expression.release(), new IR::Temp(name),
                         trueLabel, falseLabel);
}

}
