#include <VariableGraph.h>

namespace RegLifecycle {

void VariableGraph::Node::AddConnection(VariableGraph::Node* with)
{
    if(with == this) {
        return;
    }
    if(moves.find(with) != moves.end()) {
        moves.erase(with);
        with->moves.erase(this);
    }
    if(connections.insert(with).second) {
        with->connections.insert(this);
    }
}

void VariableGraph::Node::AddMove(VariableGraph::Node* with)
{
    if(with == this) {
        return;
    }
    if(connections.find(with) != connections.end()) {
        return;
    }

    if(moves.insert(with).second) {
        with->moves.insert(this);
    }
}

VariableGraph::VariableGraph(const LifecycleGraph& graph)
{
    nodes.reserve(graph.GetRegs().size());
    for(auto& t: graph.GetRegs()) {
        nodes.insert({t, std::make_unique<Node>(t)});
    }
    buildGraph(graph);
}

void VariableGraph::buildGraph(const LifecycleGraph& graph)
{
    for(auto& node: graph.GetNodesList()) {
        auto instruction = node->GetInstruction();
        auto& outs = node->GetOut();
        auto move = dynamic_cast<const CG::MoveInstruction*>(instruction);
        if(move != nullptr && move->PureMove()) {
            auto& defined = instruction->DefinedVars();
            auto& used = instruction->UsedVars();
            assert(defined.size() == 1);
            assert(used.size() == 1);

            nodes[*defined[0]]->AddMove(nodes[*used[0]].get());
            for(auto& t: outs) {
                if(t != *used[0]) {
                    nodes[*defined[0]]->AddConnection(nodes[t].get());
                }
            }
        } else {
            for(auto& d: instruction->DefinedVars()) {
                for(auto& t: outs) {
                    nodes[*d]->AddConnection(nodes[t].get());
                }
            }
        }
    }
}





}
