#pragma once

#include <LifecycleGraph.h>

namespace RegLifecycle {

class VariableGraph {
public:
    class INode {
    public:
        std::unordered_set<INode*>& ConnectedNodes() { return connections; }
        std::unordered_set<INode*>& Moves() { return moves; }

        void JustAddConnection(INode* with);
        void JustAddMove(INode* with);

        void JustRemoveConnection(INode* with);
        void JustRemoveMove(INode* with);

        const int Color() const { return color; }

        virtual ~INode() = default;
        virtual void SetColor(int c) = 0;

        virtual const IR::Temp& GetReg() const = 0;

    protected:
        int color;
        std::unordered_set<INode*> connections;
        std::unordered_set<INode*> moves;
    };

    class Node : public INode {
    public:
        explicit Node(const IR::Temp& temp): temp(temp) { color = -3; }

        virtual void SetColor(int c) override { color = c; }

        void AddConnection(Node* with);
        void AddMove(Node* with);

        virtual const IR::Temp& GetReg() const override { return temp; }

    private:
        IR::Temp temp;

        Node(const Node& other) = delete;
    };

    class MergedNode : public INode {
    public:
        MergedNode(INode* a, INode* b);
        ~MergedNode();

        virtual void SetColor(int c) override
        {
            color = c;
            l->SetColor(c);
            r->SetColor(c);
        }

        virtual const IR::Temp& GetReg() const override {
            assert(false);
            return IR::Temp();
        }

        INode* GetL() const { return l; }
        INode* GetR() const { return r; }

    private:
        INode* l;
        INode* r;

        MergedNode(const MergedNode& other) = delete;
    };

    explicit VariableGraph(const LifecycleGraph& graph, const IR::Temp* fp);

    const std::unordered_map<IR::Temp, std::unique_ptr<Node> >& GetNodes() const { return nodes; }

    const VariableGraph::INode* GetBadNode() const { return badNode; }

private:
    std::unordered_map<IR::Temp, std::unique_ptr<Node> > nodes;
    std::unordered_set<INode*> dynamicNodes;

    void buildGraph(const LifecycleGraph& graph, const IR::Temp* fp);
    void colorizeGraph(int colorsNumber);

    VariableGraph::INode* badNode;
};

}
