#pragma once
#include <Instruction.h>
#include <unordered_set>

namespace CG = CodeGeneration;

namespace RegLifecycle {

class LifecycleNode {
public:
    LifecycleNode(const CG::IInstruction* instruction, const std::vector<IR::Temp>& used,
                  const std::vector<IR::Temp>& defined);

    const CG::IInstruction* GetInstruction() const { return instruction; }
    const std::unordered_set<IR::Temp>& GetOut() const { return out; }

    bool Update();

    void AddConnection(LifecycleNode* target) {
        next.push_back(target);
        target->prev.push_back(this);
    }

    std::string Format() const;

    const std::vector<const LifecycleNode*>& GetNext() const { return next; }
    const std::vector<const LifecycleNode*>& GetPrev() const { return prev; }

private:
    const CG::IInstruction* instruction;

    bool isMove;
    std::unordered_set<IR::Temp> used;
    std::unordered_set<IR::Temp> defined;

    std::vector<const LifecycleNode*> next;
    std::vector<const LifecycleNode*> prev;

    std::unordered_set<IR::Temp> in;
    std::unordered_set<IR::Temp> out;
};

}
