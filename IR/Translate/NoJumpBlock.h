#pragma once

#include "SubtreeWrapper.h"

using namespace IR;

namespace IRTranslate {

class NoJumpBlock {
public:
    explicit NoJumpBlock(std::unique_ptr<LinearTree>&& tree);
    NoJumpBlock(NoJumpBlock&& other);

    std::unique_ptr<LinearTree>&& ReleaseTree() { return std::move(tree); }
    const LinearTree* GetTree() const { return tree.get(); }
    const Label* GetInLabel() const { return inLabel; }
    const Label* GetOutLabel() const { return outLabel; }

private:
    std::unique_ptr<LinearTree> tree;
    const Label* inLabel;
    const Label* outLabel;
};

typedef std::vector<std::unique_ptr<NoJumpBlock>> BaseBlocks;

class NoJumpTree {
public:
    explicit NoJumpTree(LinearTree& fullTree);
    NoJumpTree(NoJumpTree&& other) : blocks(std::move(other.blocks)) {}

    LinearTree BuildTree();

private:
    BaseBlocks blocks;

    void addWithJumpAtTheEnd(const Label* label, std::unique_ptr<LinearTree>& currentTree);
    void addEndedWith(const Label* label, std::unique_ptr<LinearTree>& currentTree,
                      LinearTree& fullTree);
    void addLast(std::unique_ptr<LinearTree>& currentTree);
    void deleteUnusedLabels(LinearTree& tree);
};

typedef std::unordered_map<const StringSymbol*, NoJumpTree> NoJumpBlocksForest;

}
