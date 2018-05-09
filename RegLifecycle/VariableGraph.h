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

    private:
        IR::Temp temp;
        std::unordered_set<const Node*> connections;
        std::unordered_set<const Node*> moves;

        Node(const Node& other) = delete;
    };

    VariableGraph(const LifecycleGraph& graph);

private:
    std::unordered_map<IR::Temp, std::unique_ptr<Node> > nodes;

    void buildGraph(const LifecycleGraph& graph);
};

}
