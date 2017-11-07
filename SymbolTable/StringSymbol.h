#ifndef STRINGSYMBOL_H
#define STRINGSYMBOL_H

#include <string>
#include <memory>

class StringSymbol {
public:
    static StringSymbol* GetIntern(const std::string& name);
    const std::string& GetString() const { return body; }

private:
    std::string body;

    StringSymbol(std::string name) : body(name) { }

    StringSymbol( const StringSymbol& ) = delete;
    void operator=( const StringSymbol& ) = delete;
};

#endif // STRINGSYMBOL_H
