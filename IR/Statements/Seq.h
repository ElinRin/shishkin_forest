#pragma once

#include <memory>

#include "IStm.h"

namespace IR {

class Seq : public IStm {
public:
    std::unique_ptr<const IStm> LeftStm;
    std::unique_ptr<const IStm> RightStm;

    Seq(const IStm* leftStm, const IStm* rightStm, const Coords& coords=Coords()) :
        LeftStm(leftStm),
        RightStm(rightStm),
        coords(coords)
    {}

    ACCEPT_IR_VISITOR
};

}
