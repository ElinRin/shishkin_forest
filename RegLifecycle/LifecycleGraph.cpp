#include <LifecycleGraph.h>

namespace RegLifecycle {

LifecycleGraph::LifecycleGraph(CodeGeneration::InstructionList& list)
{
    for(auto& instructionPtr: list.Instructions) {
        IR::TempList used;
        for(auto var: instructionPtr->UsedVars()) {
            auto inserted = regs.insert(std::make_unique<IR::Temp>(*var));
            used.push_back(inserted.first->get());
        }
        IR::TempList defined;
        for(auto var: instructionPtr->DefinedVars()) {
            auto inserted = regs.insert(std::make_unique<IR::Temp>(*var));
            defined.push_back(inserted.first->get());
        }
        nodes.push_back(LifecycleNode(instructionPtr.get(), used, defined));
    }
}

void LifecycleGraph::BuildLifecycle()
{
    for(bool changed = true; changed;) {
        changed = false;
        for(auto& n: nodes) {
            changed |= n.Update();
        }
    }
}

}
