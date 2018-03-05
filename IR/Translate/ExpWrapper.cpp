#include "ExpWrapper.h"

#include <string>

#include "Exp.h"
#include "JumpC.h"
#include "Temp.h"

namespace IRTranslate {

const IR::IStm* ExpWrapper::ToStm()
{
    return new IR::Exp(expression.release());
}

const IR::IStm* ExpWrapper::ToConditional(IR::JumpC::TJumpType type,
                                          const IR::Label* trueLabel)
{
    std::string name = "true";
    return new IR::JumpC(type, expression.release(), new IR::Temp(name),
                         trueLabel);
}

}
