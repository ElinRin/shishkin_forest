#pragma once

#include "IExp.h"

namespace IR {

class Binop : public IExp {
    enum TBinop {
      TB_PLUS,
      TB_MINUS,
      TB_OR,
      TB_AND,
      TB_MUL
    };

    const TBinop Operation;
    std::unique_ptr<const IR::IExp> LeftExpression;
    std::unique_ptr<const IR::IExp> RightExpression;

    Binop(TBinop operation, const IR::IExp* leftExpression,
          const IR::IExp* rightExpression) :
        Operation(operation),
        LeftExpression(leftExpression),
        RightExpression(rightExpression) { }
};

}
