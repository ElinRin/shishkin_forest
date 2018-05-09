#include <VariableGraphPrinter.h>

namespace RegLifecycle {

void VariableGraphPrinter::PrintPrefix()
{
    Dot << "strict graph g {\n" <<
           "graph [ rankdir = TD ];\n"
           "node [\n"
           "fontsize = \"16\"\n"
           "shape = \"ellipse\"\n"
           "];\n"
           "edge [\n"
           "];\n" << std::endl;
}

void VariableGraphPrinter::Print(const RegLifecycle::VariableGraph& graph)
{
    std::unordered_map<IR::Temp, std::string> nodeToName;
    for(auto& node: graph.GetNodes()) {
        const IR::Temp& t = node.first;
        nodeToName.insert({t, AddNode(t.Name() + " [" + std::to_string(t.Id) + "]")});
    }

    for(auto& node: graph.GetNodes()) {
        const IR::Temp& t = node.first;
        ParentName = nodeToName[t];
        for(auto& to: node.second->ConnectedNodes()) {
            AddArrow(nodeToName[to->GetReg()]);
        }
        for(auto& to: node.second->Moves()) {
            AddArrow(nodeToName[to->GetReg()], true);
        }
    }
}

void VariableGraphPrinter::PrintPostfix()
{
    PrintArrows(false);
    Dot << "}" << std::endl;
}

}
