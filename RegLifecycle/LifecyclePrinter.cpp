#include <LifecyclePrinter.h>

namespace RegLifecycle {

void LifecyclePrinter::PrintPrefix()
{
    Dot << "digraph g {\n" <<
           "graph [ rankdir = LR ];\n"
           "node [\n"
           "fontsize = \"16\"\n"
           "shape = \"ellipse\"\n"
           "];\n"
           "edge [\n"
           "];\n" << std::endl;
}

void LifecyclePrinter::Print(const std::vector<std::unique_ptr<LifecycleNode> >& nodes)
{
    std::unordered_map<const LifecycleNode*, const std::string> nodeToName;
    for(auto& node: nodes) {
            const std::string name = AddNode(node->Format());
            nodeToName.insert({node.get(), name});
    }
    for(auto& node: nodes) {
        ParentName = nodeToName[node.get()];
        for(auto to: node->GetNext()) {
            AddArrow(nodeToName[to]);
        }
    }
}

void LifecyclePrinter::PrintPostfix()
{
    PrintArrows();
    Dot << "}" << std::endl;
}

}
