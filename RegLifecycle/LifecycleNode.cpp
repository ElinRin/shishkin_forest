#include <LifecycleNode.h>

namespace RegLifecycle {

LifecycleNode::LifecycleNode(const CodeGeneration::IInstruction* instruction,
                             const IR::TempList& used, const IR::TempList& defined):
    instruction(instruction)
{
    for(auto u: used) {
        this->used.insert(u);
    }
    for(auto d: defined) {
        this->defined.insert(d);
    }
}

bool LifecycleNode::Update()
{
    bool inserted = false;
    for(auto t: used) {
        inserted |= in.insert(t).second;
    }
    for(auto outNode: next) {
        for(auto inNode: outNode->in) {
            inserted |= out.insert(inNode).second;
        }
    }

    for(auto outNode: out) {
        if(defined.find(outNode) == defined.end()) {
            inserted |= in.insert(outNode).second;
        }
    }
    return inserted;
}

}
