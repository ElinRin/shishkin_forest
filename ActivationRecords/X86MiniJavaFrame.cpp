#include "Frame.h"
#include "Symbol.h"
#include "Access.h"
#include "InRegAccess.h"
#include "InFrameAccess.h"
#include <unordered_map> 
#include <vector>
#include <string>
#include <assert.h>

#include "X86MiniJavaFrame.h"

namespace ActivationRecords {

static const int MAX_IN_REG = 2;
static const char* const EXIT_ADDRESS_NAME= "@EXIT_ADDRESS@";
static const char* const RETURN_ADDRESS_NAME= "@RETURN_ADDRESS@";

static const int intSize = 1 * X86MiniJavaFrame::WORD_SIZE;
static const int booleanSize = 1 * X86MiniJavaFrame::WORD_SIZE;
static const int referenceSize = 1 * X86MiniJavaFrame::WORD_SIZE;

int typeSize(SymbolTable::T_VariableType type) {
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

void X86MiniJavaFrame::AddFormal( const SymbolTable::VariableInfo& name) {
    IAccess* var = createFormal(T_RecordsType::RT_Formal, typeSize(name.GetType().GetType()));
    formalAccess.insert({name.GetName()->GetString(), var});
    formalList.push_back(var);
}

void X86MiniJavaFrame::AddLocal( const SymbolTable::VariableInfo& name) {
    IAccess* var = new InFrameAccess(RT_Formal, typeSize(name.GetType().GetType()), localAccess.size() );
    localAccess.insert({name.GetName()->GetString(), var});
    localTopPointer += typeSize(name.GetType().GetType());
}

void X86MiniJavaFrame::AddAddressExit() {
    IAccess* var = createFormal(T_RecordsType::RT_AddressExit, referenceSize);
    formalAccess.insert({EXIT_ADDRESS_NAME, var});
    formalList.push_back(var);
    this->addressExitIndex = formalList.size();
}

void X86MiniJavaFrame::AddAddressReturnValue(SymbolTable::T_VariableType type) {
    IAccess* var = createFormal(T_RecordsType::RT_AddressExit, referenceSize);
    formalAccess.insert({RETURN_ADDRESS_NAME, var});
    formalList.push_back(var);
    this->addressReturnValueIndex = formalList.size();
}

int X86MiniJavaFrame::FormalsCount() const {
    return formalList.size();
}

const IAccess* X86MiniJavaFrame::Formal( int index ) const {
    return formalList[index];
}

const IAccess* X86MiniJavaFrame::FindLocalOrFormal( const StringSymbol* name ) const {
    auto res = localAccess.find(name->GetString());
    if( res == localAccess.end() ) {
        res = formalAccess.find(name->GetString());
        assert(res != formalAccess.end());
    }
    return res->second;
}

const int X86MiniJavaFrame::FormalSize(int index) const {
    return formalList[index]->GetSize();
}

const int X86MiniJavaFrame::FormalSize(const StringSymbol* name) const {
    auto res = formalAccess.find(name->GetString());
    assert(res != formalAccess.end());
    return res->second->GetSize();
}

const Temp X86MiniJavaFrame::FP() const {
    return Temp(formalTopPointer);
}

const Temp X86MiniJavaFrame::SP() const
{
    return Temp(FP().GetAddress() + localTopPointer);
}

IAccess* X86MiniJavaFrame::createFormal(T_RecordsType type, int size)
{
    if(formalList.size() < MAX_IN_REG) {
        return new InRegAccess(type, size, formalList.size());
    } else {
        formalTopPointer += size;
        return new InFrameAccess(type, size, formalList.size() - MAX_IN_REG);
    }
}

}

