#pragma once

#include "Frame.h"
#include "Symbol.h"
#include "Access.h"
#include <unordered_map> 
#include <vector>
#include <string>

namespace ActivationRecords {

class X86MiniJavaFrame : public IFrame {
public:
    void AddFormal( const SymbolTable::Symbol* name);
    void AddLocal( const SymbolTable::Symbol* name);
    int FormalsCount() const;
    const IAccess* Formal( int index ) const;
    const IAccess* FindLocalOrFormal( const SymbolTable::Symbol* name ) const;
    const int FormalSize(int index) const;
    const int FormalSize(const SymbolTable::Symbol* name) const;
private:
    std::vector<IAccess* access> formalList;
    std::unordered_map<std::string, IAccess* access> stack;
    std::unordered_map<std::string, IAccess* access> registers;
};

}
