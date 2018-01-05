#pragma once

#include <memory>

#include "Label.h"
#include "IStm.h"

namespace IR {

class Jump : public IStm {
public:
    const Label* LabelToJump;

    Jump(const Label* label):
        LabelToJump(label)
    {}
};

}
