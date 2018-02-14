#pragma once

#include "common.h"
#include "TypeSpec.h"

namespace ActivationRecords {

class X86MiniJavaTypeSpec : public ITypeSpec {
public:
    virtual int TypeSize(SymbolTable::T_VariableType type) const override;
    virtual int ReferenceSize() const override;
    virtual int WordSize() const override;
private:
    short _sentiel = 0;
};

}
