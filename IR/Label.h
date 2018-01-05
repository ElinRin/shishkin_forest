#pragma once

#include <string>

namespace IR {

class Label {
public:
    static Label* GetLabel(std::string& name);
    static Label* GetNextEnumeratedLabel();

    std::string GetName() { return name; }


private:
    Label(std::string& name);
    std::string name;
};

}
