#include "X86MiniJavaTypeSpec.h"

#include "assert.h"

namespace ActivationRecords {

static const int WORD_SIZE = 4;
static const int intSize = 1 * WORD_SIZE;
static const int booleanSize = 1 * WORD_SIZE;
static const int referenceSize = 1 * WORD_SIZE;

int X86MiniJavaTypeSpec::TypeSize(SymbolTable::T_VariableType type) const
{

    switch (type) {
        case SymbolTable::VT_Int:
            return intSize;
        case SymbolTable::VT_IntArray:
            return referenceSize;
        case SymbolTable::VT_Boolean:
            return booleanSize;
        case SymbolTable::VT_UserClass:
            return referenceSize;
        default:
            assert(false);
    }
}

int X86MiniJavaTypeSpec::ReferenceSize() const
{
    return referenceSize;
}

int X86MiniJavaTypeSpec::WordSize() const
{
    return WORD_SIZE;
}

}
