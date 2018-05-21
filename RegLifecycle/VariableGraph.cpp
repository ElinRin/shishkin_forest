#include <set>
#include <stack>
#include <iostream>

#include <VariableGraph.h>

namespace RegLifecycle {

void VariableGraph::INode::JustAddConnection(VariableGraph::INode *with) {
    connections.insert(with);
}

void VariableGraph::INode::JustAddMove(VariableGraph::INode *with) {
    moves.insert(with);
}

void VariableGraph::INode::JustRemoveConnection(VariableGraph::INode *with) {
    if (connections.find(with) != connections.end()) {
        connections.erase(with);
    }
}

void VariableGraph::INode::JustRemoveMove(VariableGraph::INode *with) {
    if (moves.find(with) != moves.end()) {
        moves.erase(with);
    }
}

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
    AddConnection(with);
    return;
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

VariableGraph::VariableGraph(const LifecycleGraph& graph, const IR::Temp* fp): badNode(nullptr)
{
    nodes.reserve(graph.GetRegs().size());
    for(auto& t: graph.GetRegs()) {
        nodes.insert({t, std::make_unique<Node>(t)});
        dynamicNodes.insert(nodes.find(t)->second.get());
    }
    buildGraph(graph, fp);
    colorizeGraph(4);
}

void VariableGraph::buildGraph(const LifecycleGraph& graph, const IR::Temp* fp)
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

    for (auto& item: nodes) {
        if (item.first == (*fp)) {
            for (auto& connectedNode : item.second->ConnectedNodes()) {
                connectedNode->JustRemoveConnection(item.second.get());
            }
            for (auto& connectedNode : item.second->Moves()) {
                connectedNode->JustRemoveMove(item.second.get());
            }
            dynamicNodes.erase(item.second.get());
            nodes.erase(item.first);
            break;
        }
    }
}

VariableGraph::MergedNode::MergedNode(VariableGraph::INode *a, VariableGraph::INode *b): l(a), r(b)
{
    color = -3;
    for (const auto& node : a->ConnectedNodes()) {
        assert(node != b);
        JustAddConnection(node);
        node->JustRemoveConnection(a);
        node->JustAddConnection(this);
    }
    for (const auto& node : b->ConnectedNodes()) {
        assert(node != a);
        JustAddConnection(node);
        node->JustRemoveConnection(b);
        node->JustAddConnection(this);
    }

    for (const auto& node : a->Moves()) {
        if (node != b) {
            node->JustRemoveMove(a);
            if (connections.find(node) == connections.end()) {
                JustAddMove(node);
                node->JustAddMove(this);
            }
        }
    }
    for (const auto& node : b->Moves()) {
        if (node != a) {
            node->JustRemoveMove(b);
            if (connections.find(node) == connections.end()) {
                JustAddMove(node);
                node->JustAddMove(this);
            }
        }
    }
}

VariableGraph::MergedNode::~MergedNode() {
    for (const auto& node : connections) {
        node->JustRemoveConnection(this);
    }
    for (const auto& node : moves) {
        node->JustRemoveMove(this);
    }
    for (const auto& node : l->ConnectedNodes()) {
        node->JustAddConnection(l);
    }
    for (const auto& node : r->ConnectedNodes()) {
        node->JustAddConnection(r);
    }
    for (const auto& node : l->Moves()) {
        node->JustAddMove(l);
    }
    for (const auto& node : r->Moves()) {
        node->JustAddMove(r);
    }

}

void VariableGraph::colorizeGraph(int colorsNumber) {
    std::stack<VariableGraph::INode*> stackNodes;
    std::stack<std::unique_ptr<VariableGraph::MergedNode> > mergedNodes;
    std::stack<char> type;
    bool isSimplified;
    bool isMerged;
    bool isFrozen;
    bool isSpilled;

    do {
        isSimplified = isMerged = isFrozen = isSpilled = false;
        for (auto &node : dynamicNodes) {
            assert(node->Color() == -3);
            for (auto& connectedNode : node->ConnectedNodes()) {
                assert(dynamicNodes.find(connectedNode) != dynamicNodes.end());
            }
            for (auto& connectedNode : node->Moves()) {
                assert(dynamicNodes.find(connectedNode) != dynamicNodes.end());
            }
        }

        /* simplify phase */ {
            //std::cout << "Try simplify" << std::endl;
            std::stack<VariableGraph::INode*> eraseNodes;
            for (auto &node : dynamicNodes) {
                if (node->Color() == -3 && node->Moves().empty()) {
                    if (node->ConnectedNodes().size() < colorsNumber) {
                        node->SetColor(-2);
                        stackNodes.push(node);
                        type.push('s');
                        eraseNodes.push(node);
                        for (auto& connectedNode : node->ConnectedNodes()) {
                            connectedNode->JustRemoveConnection(node);
                        }
                        isSimplified = true;
                        break;
                    }
                }
            }
            while (!eraseNodes.empty()) {
                dynamicNodes.erase(eraseNodes.top());
                eraseNodes.pop();
            }
            if (isSimplified) {
                //std::cout << "Simplify" << std::endl;
                continue;
            }
        }

        /* Briggs merge phase */ {
            //std::cout << "Try merge" << std::endl;
            INode* a = nullptr;
            INode* b = nullptr;
            for (auto &node : dynamicNodes) {
                if (node->Color() == -3 && !node->Moves().empty()) {
                    a = node;
                    for (const auto& moveNode : node->Moves()) {
                        std::set<const INode*> commonConnections;
                        for (const auto& connectedNode : node->ConnectedNodes()) {
                            commonConnections.insert(connectedNode);
                        }
                        for (const auto& connectedNode : moveNode->ConnectedNodes()) {
                            commonConnections.insert(connectedNode);
                        }
                        if (commonConnections.size() < colorsNumber) {
                            b = moveNode;
                            break;
                        }
                    }
                    if (b != nullptr) {
                        break;
                    }
                }
            }
            if (b != nullptr) {
                isMerged = true;
                mergedNodes.push(std::make_unique<VariableGraph::MergedNode>(a, b));
                type.push('m');
                mergedNodes.top()->SetColor(-3);
                dynamicNodes.erase(a);
                dynamicNodes.erase(b);
                dynamicNodes.insert(mergedNodes.top().get());
            }
            if (isMerged) {
                //std::cout << "Merge" << std::endl;
                continue;
            }
        }

        /* freeze phase */ {
            //std::cout << "Try freeze" << std::endl;
            for (auto& node : dynamicNodes) {
                if (node->Color() == -3 && node->Moves().size() == 1) {
                    INode* moveNode = *(node->Moves().begin());
                    moveNode->JustRemoveMove(node);
                    node->JustRemoveMove(moveNode);
                    moveNode->JustAddConnection(node);
                    node->JustAddConnection(moveNode);
                    isFrozen = true;
                    break;
                }
            }
            if (isFrozen) {
                //std::cout << "Freeze" << std::endl;
                continue;
            }
        }

        /* Spill phase */ {
            //std::cout << "Try spill" << std::endl;
            std::stack<VariableGraph::INode*> eraseNodes;
            for (auto &node : dynamicNodes) {
                if (node->Color() == -3 && node->Moves().empty()) {
                    assert (node->ConnectedNodes().size() >= colorsNumber);
                    node->SetColor(-1);
                    stackNodes.push(node);
                    type.push('s');
                    eraseNodes.push(node);
                    for (auto& connectedNode : node->ConnectedNodes()) {
                        connectedNode->JustRemoveConnection(node);
                    }
                    isSpilled = true;
                    break;
                }
            }
            while (!eraseNodes.empty()) {
                dynamicNodes.erase(eraseNodes.top());
                eraseNodes.pop();
            }
            if (isSpilled) {
                //std::cout << "Spill" << std::endl;
                continue;
            }
        }
    } while (isSimplified || isMerged || isFrozen || isSpilled);

    for (auto &node : dynamicNodes) {
        std::cout << node << std::endl;
        for (auto &moveNode : node->Moves()) {
            std::cout << "  " << moveNode << std::endl;
        }
        for (auto &connectedNode : node->ConnectedNodes()) {
            std::cout << "    " << connectedNode << std::endl;
        }
    }
    assert(dynamicNodes.empty());

    while (!type.empty()) {
        if (type.top() == 's') {
            std::set<int> connectedColors;
            for (auto &connectedNode : stackNodes.top()->ConnectedNodes()) {
                connectedNode->JustAddConnection(stackNodes.top());
                connectedColors.insert(connectedNode->Color());
            }
            int color = 0;
            for (; color < colorsNumber; ++color) {
                if (connectedColors.find(color) == connectedColors.end()) {
                    break;
                }
            }

            if (badNode == nullptr && color >= colorsNumber){
                badNode = stackNodes.top();
                //assert(false);
            }

            stackNodes.top()->SetColor(color);
            stackNodes.pop();
        } else if (type.top() == 'm') {
            mergedNodes.pop();
        }
        type.pop();
    }
}

}
