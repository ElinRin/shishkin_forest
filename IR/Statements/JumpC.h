#pragma once

#include <memory>

#include "IStm.h"
#include "IExp.h"

namespace IR {

class JumpC : public IStm {
public:
    enum TJumpType {
        TJ_EQ, TJ_NEQ, TJ_LT
    };

    std::unique_ptr<const AR::Label> TrueLabel;
    std::unique_ptr<const AR::Label> FalseLabel;
    std::unique_ptr<const IR::IExp> ConditionLeftExpression;
    std::unique_ptr<const IR::IExp> ConditionRightExpression;
    const TJumpType JumpType;


    JumpC(const TJumpType jumpType,
          const IR::IExp* conditionLeftExpression,
          const IR::IExp* conditionRightExpression,
          const AR::Label* trueLabel,
          const AR::Label* falseLabel) :
        JumpType(jumpType), ConditionLeftExpression(conditionLeftExpression),
        ConditionRightExpression(conditionRightExpression), TrueLabel(trueLabel), FalseLabel(falseLabel) { }
};

}
