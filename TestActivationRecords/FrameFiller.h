#pragma once

#include "Table.h"

namespace ActivationRecords {

class FrameFiller {
public:
    FrameFiller(SymbolTable::Table* table) : table(table), filled(false) {}

    void Fill();

private:
    SymbolTable::Table* table;
    bool filled;
};

}
