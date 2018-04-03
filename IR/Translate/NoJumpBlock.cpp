#include "NoJumpBlock.h"

#include "IRNodeTypes.h"
#include "IRVisitor.h"

#include <unordered_map>
#include <unordered_set>
#include <set>

namespace IRTranslate {

class BaseLabelVisitor :  public IIRVisitor {
public:
    BaseLabelVisitor() : label(nullptr) {}

    const Label* GetLabel() const { return label; }

    // Expressions
    virtual void Visit(IR::Unaryop* node) override {}
    virtual void Visit(IR::Binop* node) override {}
    virtual void Visit(IR::Call* node) override {}
    virtual void Visit(IR::Const* node) override {}
    virtual void Visit(IR::Eseq* node) override {}
    virtual void Visit(IR::Mem* node) override {}
    virtual void Visit(IR::Name* node) override {}
    virtual void Visit(IR::Temp* node) override {}
    virtual void Visit(IR::ExpList* node) override {}
    // Statements
    virtual void Visit(IR::Exp* node) override {}
    virtual void Visit(IR::Jump* node) override {}
    virtual void Visit(IR::JumpC* node) override {}
    virtual void Visit(IR::LabelStm* node) override {}
    virtual void Visit(IR::Move* node) override {}
    virtual void Visit(IR::Seq* node) override {}

    virtual void Visit(StmWrapper* node) override {}
    virtual void Visit(ExpWrapper* node) override {}

protected:
    void SetLabel(const Label* newLabel) { label = newLabel; }

private:
    const Label* label;
};

class InLabelVisitor : public BaseLabelVisitor {
public:
    virtual void Visit(IR::LabelStm* node) override { SetLabel(node->LabelName); }
};

class OutLabelVisitor : public BaseLabelVisitor {
public:
    virtual void Visit(IR::Jump* node) override { SetLabel(node->LabelToJump); }
    virtual void Visit(IR::JumpC* node) override { SetLabel(node->TrueLabel); }
};



NoJumpBlock::NoJumpBlock(std::unique_ptr<IRTranslate::LinearTree>&& _tree) :
    tree(std::move(_tree)),
    inLabel(nullptr),
    outLabel(nullptr)
{
    assert(tree->size() > 0);
    InLabelVisitor inVisitor;
    tree->at(0)->AcceptVisitor(&inVisitor);
    inLabel = inVisitor.GetLabel();

    OutLabelVisitor outVisitor;
    tree->at(tree->size() - 1)->AcceptVisitor(&outVisitor);
    outLabel = outVisitor.GetLabel();
}

NoJumpBlock::NoJumpBlock(NoJumpBlock&& other):
    tree(std::move(other.tree)),
    inLabel(other.inLabel),
    outLabel(other.outLabel)
{
}


NoJumpTree::NoJumpTree(LinearTree& fullTree)
{
    std::unique_ptr<LinearTree> currentTree(new LinearTree());
    while(fullTree.size() > 0) {
        InLabelVisitor inVisitor;
        fullTree[0]->AcceptVisitor(&inVisitor);
        if(inVisitor.GetLabel() != nullptr && currentTree->size() > 0) {
            addWithJumpAtTheEnd(inVisitor.GetLabel(), currentTree);
            continue;
        }
        currentTree->push_back(std::unique_ptr(std::move(fullTree[0])));
        fullTree.erase(fullTree.begin());
        OutLabelVisitor outVisitor;
        currentTree->rbegin()->get()->AcceptVisitor(&outVisitor);
        if(outVisitor.GetLabel() != nullptr && fullTree.size() > 0) {
            addEndedWith(outVisitor.GetLabel(), currentTree, fullTree);
            continue;
        }
    }
    if(currentTree->size() > 0) {
        addLast(currentTree);
    }
}

LinearTree NoJumpTree::BuildTree()
{
    LinearTree tree;
    for(int i = 0; i < blocks.size(); ++i) {
        int from = 0;
        bool missFirst = i < blocks.size() - 1 && blocks[i]->GetOutLabel() == blocks[i+1]->GetInLabel();
        int to = blocks[i]->GetTree()->size();
        if(missFirst) {
            --to;
        }
        std::unique_ptr<LinearTree> subtree(blocks[i]->ReleaseTree().release());
        for(;from < to; ++from) {
            tree.push_back(std::unique_ptr<IR::IStm>(subtree->at(from).release()));
        }
    }
    deleteUnusedLabels(tree);
    return tree;
}

void NoJumpTree::deleteUnusedLabels(LinearTree& tree)
{
    std::unordered_set<const Label*> usedLabels;
    std::unordered_map<const Label*, std::vector<int>> unusedLabels;
    for(int i = 0; i < tree.size(); ++i) {
        auto& treeElement = tree[i];
        InLabelVisitor inVisitor;
        treeElement->AcceptVisitor(&inVisitor);
        OutLabelVisitor outVisitor;
        treeElement->AcceptVisitor(&outVisitor);
        if(outVisitor.GetLabel() != nullptr) {
            usedLabels.insert(outVisitor.GetLabel());
            auto found = unusedLabels.find(outVisitor.GetLabel());
            if(found != unusedLabels.end()) {
                unusedLabels.erase(found);
            }
        }
        const Label* inLabel = inVisitor.GetLabel();
        if(inLabel != nullptr && usedLabels.find(inLabel) == usedLabels.end()) {
            if(unusedLabels.find(inLabel) == unusedLabels.end()) {
                unusedLabels.insert(std::make_pair(inLabel, std::vector<int>()));
            }
            unusedLabels.find(inLabel)->second.push_back(i);
        }
    }
    std::set<int> indices;
    for(auto positions = unusedLabels.begin(); positions != unusedLabels.end(); ++positions) {
        for(auto pos = positions->second.begin(); pos != positions->second.end(); ++pos) {
            indices.insert(*pos);
        }
    }
    int deleted = 0;
    for(auto pos = indices.begin(); pos != indices.end(); ++pos) {
        tree.erase(tree.begin()+*pos-deleted);
        ++deleted;
    }
}

void NoJumpTree::addLast(std::unique_ptr<LinearTree>& currentTree)
{
    blocks.push_back(std::make_unique<NoJumpBlock>(std::move(currentTree)));
}

void NoJumpTree::addEndedWith(const Label* label, std::unique_ptr<LinearTree>& currentTree, LinearTree& fullTree)
{
    if(fullTree.size() == 0) {
        blocks.push_back(std::make_unique<NoJumpBlock>(std::move(currentTree)));
        currentTree.reset(new LinearTree());
        return;
    }
    InLabelVisitor inVisitor;
    fullTree[0]->AcceptVisitor(&inVisitor);
    if(inVisitor.GetLabel() == label) {
        blocks.push_back(std::make_unique<NoJumpBlock>(std::move(currentTree)));
        currentTree.reset(new LinearTree());
        return;
    }
    label = Label::GetNextEnumeratedLabel();
    currentTree->push_back(std::make_unique<Jump>(label));
    blocks.push_back(std::make_unique<NoJumpBlock>(std::move(currentTree)));
    currentTree.reset(new LinearTree());
    currentTree->push_back(std::make_unique<LabelStm>(label));
}

void NoJumpTree::addWithJumpAtTheEnd(const Label* label, std::unique_ptr<LinearTree>& currentTree)
{
    OutLabelVisitor outVisitor;
    currentTree->rbegin()->get()->AcceptVisitor(&outVisitor);
    if(outVisitor.GetLabel() != label) {
        currentTree->push_back(std::make_unique<Jump>(label));
    }
    blocks.push_back(std::make_unique<NoJumpBlock>(std::move(currentTree)));
    currentTree.reset(new LinearTree());
}

}
