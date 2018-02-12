#pragma once

#include "Table.h"

namespace ActivationRecords {

class FrameFiller {
public:    
    FrameFiller(SymbolTable::Table* table) : table(table), filled(false) {}

    void PrintFill();
    IFrame* CreateFrame(const SymbolTable::ClassInfo& classInfo,
              const SymbolTable::MethodInfo& methodInfo);

private:
    SymbolTable::Table* table;
    bool filled;
};

}
