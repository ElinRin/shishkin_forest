#pragma once

#include "Frame.h"
#include "Symbol.h"
#include "Access.h"
#include <unordered_map> 
#include <vector>
#include <string>

#include "X86MiniJavaFrame.h"

namespace ActivationRecords {

enum T_VariableType {
    VT_Int,
    VT_IntArray,
    VT_Boolean,
    VT_UserClass
};

int typeSize(T_VariableType type) {

    const int intSize = 32;
    const int booleanSize = 4;
    const int referenceSize = 4;

    switch (type) {
        case VT_Int:
            return intSize;
        case VT_IntArray:
            return referenceSize;
        case VT_Boolean:
            return booleanSize;
        case VT_UserClass:
            return referenceSize;
        default:
            return referenceSize;
    }
}

void X86MiniJavaFrame::AddFormal( const SymbolTable::Symbol* name) {
    stack.insert({name.GetName().GetString(), name}); 
    formalList.push(name);
}

void X86MiniJavaFrame::AddLocal( const SymbolTable::Symbol* name) {
    stack.insert({name.GetName().GetString(), name}); 
}

int X86MiniJavaFrame::FormalsCount() const {
    return formalList.size();
}

const IAccess* X86MiniJavaFrame::Formal( int index ) const {
    return formalList[index];
}

const IAccess* X86MiniJavaFrame::FindLocalOrFormal( const SymbolTable::Symbol* name ) const {

}

const int X86MiniJavaFrame::FormalSize(int index) const {
    SymbolTable::Symbol* name = formalList[index];
    T_VariableType type = name.GetType().GetType();
    int size = typeSize(type);
    return size;
}

const int X86MiniJavaFrame::FormalSize(const SymbolTable::Symbol* name) const {
    T_VariableType type = name.GetType().GetType();
    int size = typeSize(type);
    return size;
}
  
