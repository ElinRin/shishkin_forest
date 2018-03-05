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

    const Label* TrueLabel;
    std::unique_ptr<IExp> ConditionLeftExpression;
    std::unique_ptr<IExp> ConditionRightExpression;
    const TJumpType JumpType;


    JumpC(const TJumpType jumpType,
          IExp* conditionLeftExpression,
          IExp* conditionRightExpression,
          const Label* trueLabel, const Coords& coords=Coords()) :
        TrueLabel(trueLabel),
        ConditionLeftExpression(conditionLeftExpression),
        ConditionRightExpression(conditionRightExpression),
        JumpType(jumpType),
        coords(coords)
    {}

    ACCEPT_IR_VISITOR
};

}
