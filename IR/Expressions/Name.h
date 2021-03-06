#pragma once

#include "IExp.h"
#include "Label.h"

namespace IR {

class Name : public IExp {
public:
    const Label* LabelName;

    Name(const Coords& coords=Coords()) :
        LabelName(Label::GetNextEnumeratedLabel()),
        coords(coords)
    {}
    Name(const std::string& name, const Coords& coords=Coords()) :
        LabelName(Label::GetLabel(name)),
        coords(coords)
    {}

    virtual bool IsCommutative() const override { return true; }
    virtual bool IsAbsolutelyCommutative() const override { return true; }

    ACCEPT_IR_VISITOR
};

}
