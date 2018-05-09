#pragma once

#include <VariableGraph.h>
#include <DotPrint.h>

namespace RegLifecycle {

class VariableGraphPrinter: public DotPrint {
public:
    VariableGraphPrinter(const std::string& filename):
        DotPrint(filename)
    {}

    void PrintPrefix();
    void Print(const VariableGraph& graph);
    void PrintPostfix();
};

}
