#pragma once
#include <common.h>

    #define ACCEPT_IR_VISITOR public: virtual void AcceptVisitor(IR::IIRVisitor* visitor) const override;

namespace IR {

interface IIRVisitor;

interface INode
{
    virtual ~INode() {}
    virtual void AcceptVisitor(IIRVisitor* visitor) const = 0;
};

}
