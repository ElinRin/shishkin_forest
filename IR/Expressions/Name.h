#pragma once

#include "IExp.h"
#include "Label.h"

namespace IR {

class Name : public IExp {
public:
    const Label* LabelName;

    Name() :
        LabelName(Label::GetNextEnumeratedLabel())
    {}
    Name(const std::string& name) :
        LabelName(Label::GetLabel(name))
    {}

    ACCEPT_IR_VISITOR
};

}
