#pragma once
#include "common.h"

#include <fstream>
#include <vector>

class DotPrint {
protected:
    struct Arrow {
        std::string From;
        std::string To;
    };

    std::vector<Arrow> Arrows;
    std::ofstream Dot;
    std::string ParentName;
    int NodeCounter = 0;

    DotPrint(std::string filename) :
        Dot(filename)
    {}

    ~DotPrint() {
        Dot.close();
    }

    void AddArrow(std::string &name)
    {
        Arrows.push_back({"\"" + ParentName + "\"", "\"" + name + "\""});
    }

    std::string AddNode(std::string label)
    {
        std::string name = "name" + std::to_string(NodeCounter);
        Dot << "\"" << name << "\" [\n"
               "label = " << "\"<f0> " << label << "\"\n"
               "shape = \"record\"\n"
               "];" << std::endl;
        NodeCounter += 1;
        return name;
    }

    void PrintArrows() {
        for(int i = 0; i < Arrows.size(); ++i) {
            Arrow& arrow = Arrows[i];
            Dot << arrow.From << " -> " << arrow.To << " [\n" <<
                   "id = " << i << "\n" <<
                   "];" << std::endl;
        }
    }
};
