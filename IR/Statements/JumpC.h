#pragma once

#include <memory>

#include "IStm.h"
#include "IExp.h"
#include "Label.h"

namespace IR {

class JumpC : public IStm {
public:
    enum TJumpType {
        TJ_EQ, TJ_NEQ, TJ_LT
    };

    std::unique_ptr<const Label> TrueLabel;
    std::unique_ptr<const Label> FalseLabel;
    std::unique_ptr<const IExp> ConditionLeftExpression;
    std::unique_ptr<const IExp> ConditionRightExpression;
    const TJumpType JumpType;


    JumpC(const TJumpType jumpType,
          const IExp* conditionLeftExpression,
          const IExp* conditionRightExpression,
          const Label* trueLabel,
          const Label* falseLabel) :
        TrueLabel(trueLabel), FalseLabel(falseLabel),
        ConditionLeftExpression(conditionLeftExpression),
        ConditionRightExpression(conditionRightExpression),
        JumpType(jumpType)
    {}
};

}
