#pragma once

#include "Label.h"
#include "IStm.h"

namespace IR {

class LabelStm : public IStm {
public:
    const Label* LabelName;

    LabelStm(const Label* label) :
        LabelName(label)
    {}

    ACCEPT_IR_VISITOR
};

}
