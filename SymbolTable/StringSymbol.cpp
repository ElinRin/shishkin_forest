#include "StringSymbol.h"

#include <unordered_map>

StringSymbol* StringSymbol::GetIntern(const std::string& name)
{
    static std::unordered_map<std::string, std::unique_ptr<StringSymbol>> strings;
    auto result = strings.find(name);
    if(result == strings.end()) {
        strings.insert(std::make_pair(name ,
            std::unique_ptr<StringSymbol>(new StringSymbol(name))));
    }

    return strings.find(name)->second.get();
}
