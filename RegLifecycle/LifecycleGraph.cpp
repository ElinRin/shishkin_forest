#include <LifecycleGraph.h>

namespace RegLifecycle {

LifecycleGraph::LifecycleGraph(CodeGeneration::InstructionList& list)
{
    for(auto& instructionPtr: list.Instructions) {
        std::vector<IR::Temp> used;
        for(auto var: instructionPtr->UsedVars()) {
            auto inserted = regs.insert(IR::Temp(*var));
            used.push_back(*inserted.first);
        }
        std::vector<IR::Temp> defined;
        for(auto var: instructionPtr->DefinedVars()) {
            auto inserted = regs.insert(IR::Temp(*var));
            defined.push_back(*inserted.first);
        }

        nodes.push_back(std::make_unique<LifecycleNode>(instructionPtr.get(), used, defined));

        if(const CG::LabelInstruction* labelInstruction =
                dynamic_cast<const CG::LabelInstruction*>(instructionPtr.get()) )
        {
            labelsPositions.insert({labelInstruction->GetLabel(), nodes.size() - 1});
        }

        if(nodes.size() > 1) {
            (*(nodes.rbegin()+1))->AddConnection(nodes.rbegin()->get());
        }
    }
    for(int i = 0; i < list.Instructions.size(); ++i) {
        const IR::LabelList& labels = list.Instructions[i]->JumpTargets();
        for(auto l: labels) {
            nodes[i]->AddConnection(nodes[labelsPositions[l]].get());
        }
    }
}

LifecycleGraph::LifecycleGraph(LifecycleGraph&& other):
    nodes(std::move(other.nodes)),
    regs(std::move(other.regs)),
    labelsPositions(std::move(other.labelsPositions))
{
}

void LifecycleGraph::BuildLifecycle()
{
    std::unordered_set<LifecycleNode*> nodesToUpdate;
    for(auto& n: nodes) {
        nodesToUpdate.insert(n.get());
    }
    while(nodesToUpdate.size() > 0) {
        LifecycleNode* node = *nodesToUpdate.begin();
        nodesToUpdate.erase(nodesToUpdate.begin());
        if(node->Update()) {
            auto& toUpdateNext = node->GetNext();
            for(auto next: toUpdateNext) {
                nodesToUpdate.insert(const_cast<LifecycleNode*>(next));
            }

            auto& toUpdatePrev = node->GetPrev();
            for(auto next: toUpdatePrev) {
                nodesToUpdate.insert(const_cast<LifecycleNode*>(next));
            }
        }
    }
}

}
