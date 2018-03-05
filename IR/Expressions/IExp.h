#pragma once

#include <Node.h>

namespace IR {

interface IExp : public INode
{
    virtual bool IsCommutative() const = 0;
    virtual bool IsAbsolutelyCommutative() const = 0;
};

}
