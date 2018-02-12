#include "common.h"

#include "X86MiniJavaFrame.h"
#include "X86MiniJavaTypeSpec.h"
#include "X86MiniJavaClassStruct.h"

BIND(ActivationRecords::IFrame, ActivationRecords::X86MiniJavaFrame)
BIND(ActivationRecords::ITypeSpec, ActivationRecords::X86MiniJavaTypeSpec)
BIND(SymbolTable::IClassStruct, SymbolTable::X86MiniJavaClassStruct)
