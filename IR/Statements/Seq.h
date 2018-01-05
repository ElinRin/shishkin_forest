#pragma once

#include <memory>

#include "IStm.h"

namespace IR {

class Seq : public IStm {
public:
    std::unique_ptr<const IStm> LeftStm;
    std::unique_ptr<const IStm> RightStm;

    Seq(const IStm* leftStm, const IStm* rightStm) :
        LeftStm(leftStm),
        RightStm(rightStm)
    {}
};

}
