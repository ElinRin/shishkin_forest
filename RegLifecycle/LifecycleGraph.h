#pragma once

#include <LifecycleNode.h>

namespace RegLifecycle {

class LifecycleGraph {
public:
    LifecycleGraph(CG::InstructionList& list);

    void BuildLifecycle();

private:
    std::vector<LifecycleNode> nodes;
    std::unordered_set<std::unique_ptr<const IR::Temp>> regs;
};

}
