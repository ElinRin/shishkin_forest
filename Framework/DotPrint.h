#pragma once
#include "common.h"

#include <fstream>
#include <vector>

class DotPrint {
protected:
    struct Arrow {
        std::string From;
        std::string To;
        bool Dashed;
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

    void AddArrow(const std::string &name, bool dashed = false)
    {
        Arrows.push_back({"\"" + ParentName + "\"", "\"" + name + "\"", dashed});
    }

    const std::string AddNode(const std::string label)
    {
        std::string name = "name" + std::to_string(NodeCounter);
        Dot << "\"" << name << "\" [\n"
               "label = " << "\"<f0> " << label << "\"\n"
               "shape = \"record\"\n"
               "];" << std::endl;
        NodeCounter += 1;
        return name;
    }

    const std::string AddNode(const std::string label, const std::string color)
    {
        std::string name = "name" + std::to_string(NodeCounter);
        Dot << "\"" << name << "\" [\n"
               "label = " << "\"<f0> " << label << "\"\n"
               "shape = \"record\"\n"
               "color = \""<< color <<"\"\n"
               "];" << std::endl;
        NodeCounter += 1;
        return name;
    }

    void PrintArrows(bool direct = true) {
        for(int i = 0; i < Arrows.size(); ++i) {
            Arrow& arrow = Arrows[i];
            Dot << arrow.From << (direct ? " -> " : " -- ") << arrow.To << " [\n" <<
                   (arrow.Dashed ? "style=dashed " : "") <<
                   "id = " << i << "\n" <<
                   "];" << std::endl;
        }
    }
};
