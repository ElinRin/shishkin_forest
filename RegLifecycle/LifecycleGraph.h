#pragma once

#include <LifecycleNode.h>

namespace RegLifecycle {

class LifecycleGraph {
public:
    LifecycleGraph(CG::InstructionList& list);
    LifecycleGraph(LifecycleGraph&& other);

    void BuildLifecycle();

    const std::vector< std::unique_ptr<LifecycleNode> >& GetNodesList() const { return nodes; }

private:
    std::vector< std::unique_ptr<LifecycleNode> > nodes;
    std::unordered_set<std::unique_ptr<IR::Temp>> regs;
    std::unordered_map<const IR::Label*, int> labelsPositions;
};

typedef std::vector<LifecycleGraph> LifecyclesList;

}
