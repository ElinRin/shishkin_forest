#pragma once

#include "IExp.h"

namespace IR {

class Const : public IExp {
public:
    const int value;

    Const(int value) : value(value) { }
};

}
