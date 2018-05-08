#pragma once
#include <Instruction.h>
#include <unordered_set>

namespace CG = CodeGeneration;

namespace RegLifecycle {

class LifecycleNode {
public:
    LifecycleNode(const CG::IInstruction* instruction, const IR::TempList& used,
                  const IR::TempList& defined);

    const CG::IInstruction* GetInstruction() const { return instruction; }

    bool Update();

private:
    const CG::IInstruction* instruction;
    std::unordered_set<const IR::Temp*> used;
    std::unordered_set<const IR::Temp*> defined;

    std::vector<const LifecycleNode*> next;

    std::unordered_set<const IR::Temp*> in;
    std::unordered_set<const IR::Temp*> out;
};

}
