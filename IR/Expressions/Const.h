#pragma once

#include "IExp.h"

namespace IR {

class Const : public IExp {
public:
    const int value;

    Const(int value, const Coords& coords=Coords()) :
        value(value),
        coords(coords)
    {}

    virtual bool IsCommutative() const override { return true; }

    ACCEPT_IR_VISITOR
};

}
