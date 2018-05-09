#pragma once

#include <LifecycleNode.h>
#include <DotPrint.h>

namespace RegLifecycle {

class LifecyclePrinter: private DotPrint {
public:
    LifecyclePrinter(const std::string& name): DotPrint(name)
    {
    }

    void PrintPrefix();

    void Print(const std::vector<std::unique_ptr<LifecycleNode> >& nodes);

    void PrintPostfix();
};

}
