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
    Name(std::string& name) :
        LabelName(Label::GetLabel(name))
    {}
};

}
