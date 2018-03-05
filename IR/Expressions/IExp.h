#pragma once

#include <Node.h>

namespace IR {

interface IExp : public INode
{
    virtual bool IsCommutative() const = 0;
};

}
