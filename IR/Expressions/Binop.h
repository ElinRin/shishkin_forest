#pragma once

#include "IExp.h"

namespace IR {

class Binop : public IExp {
public:
    enum TBinop {
      TB_PLUS,
      TB_MINUS,
      TB_OR,
      TB_AND,
      TB_MUL,
      TB_MOD
    };

    const TBinop Operation;
    std::unique_ptr<IExp> LeftExpression;
    std::unique_ptr<IExp> RightExpression;

    Binop(TBinop operation, IExp* leftExpression,
          IExp* rightExpression, const Coords& coords=Coords()) :
        Operation(operation),
        LeftExpression(leftExpression),
        RightExpression(rightExpression),
        coords(coords)
    {}

    virtual bool IsCommutative() const override { return false; }
    virtual bool IsAbsolutelyCommutative() const override { return false; }

    ACCEPT_IR_VISITOR
};

}
