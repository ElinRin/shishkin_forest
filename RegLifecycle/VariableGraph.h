#pragma once

#include <LifecycleGraph.h>

namespace RegLifecycle {

class VariableGraph {
public:
    class Node {
    public:
        explicit Node(const IR::Temp& temp): temp(temp) {}

        const std::unordered_set<const Node*>& ConnectedNodes() const { return connections; }
        const std::unordered_set<const Node*>& Moves() const { return moves; }

        void AddConnection(Node* with);
        void AddMove(Node* with);
        const IR::Temp& GetReg() const { return temp; }

        int color;

    private:
        IR::Temp temp;
        std::unordered_set<const Node*> connections;
        std::unordered_set<const Node*> moves;

        Node(const Node& other) = delete;
    };

    VariableGraph(const LifecycleGraph& graph);

    const std::unordered_map<IR::Temp, std::unique_ptr<Node> >& GetNodes() const { return nodes; }

private:
    std::unordered_map<IR::Temp, std::unique_ptr<Node> > nodes;

    void buildGraph(const LifecycleGraph& graph);
    void colorizeGraph(int colorsNumber);
};

}
