#include "Label.h"

#include <unordered_map>
#include <memory>

namespace  IR {

static std::unordered_map<std::string, std::unique_ptr<Label>> Map;
static const char* const LabelPrefix = "label_";

Label* Label::GetLabel(std::string& name)
{
    auto found = Map.find(name);
    if(found != Map.end()) {
        Label* l = new Label(name);
        Map.insert(std::make_pair(name, std::unique_ptr<Label>(l)));
        return l;
    }
    return found->second.get();

}

Label* Label::GetNextEnumeratedLabel()
{
    static int id = 0;
    while(Map.find(LabelPrefix + std::to_string(id)) != Map.end()) {
        id += 1;
    }
    std::string labelName = LabelPrefix + std::to_string(id++);
    return GetLabel(labelName);
}

Label::Label(std::string& name) : name(name)
{

}

}
