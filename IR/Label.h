#pragma once

#include <string>

namespace IR {

class Label {
public:
    static Label* GetLabel(const std::string &name);
    static Label* GetNextEnumeratedLabel();

    std::string GetName() const { return name; }


private:
    Label(const std::string &name);
    std::string name;
};

}
