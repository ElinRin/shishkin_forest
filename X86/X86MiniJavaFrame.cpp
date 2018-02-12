#include "X86MiniJavaFrame.h"

#include "Symbol.h"
#include "Access.h"
#include "InRegAccess.h"
#include "InFrameAccess.h"

#include <unordered_map> 
#include <vector>
#include <string>
#include <assert.h>

namespace ActivationRecords {

static const int MAX_IN_REG = 4;
static const char* const EXIT_ADDRESS_NAME= "@EXIT_ADDRESS@";
static const char* const RETURN_ADDRESS_NAME= "@RETURN_ADDRESS@";

int X86MiniJavaFrame::TypeSize(SymbolTable::T_VariableType type) const {
    return typeSpec->TypeSize(type);
}

X86MiniJavaFrame::X86MiniJavaFrame() : typeSpec(Factory::Create<ITypeSpec>())
{
}

void X86MiniJavaFrame::AddFormal( const SymbolTable::VariableInfo& name) {
    IAccess* var = createFormal(T_RecordsType::RT_Formal, TypeSize(name.GetType().GetType()));
    formalAccess.insert(std::make_pair(name.GetName(), std::unique_ptr<IAccess>(var)));
    formalList.push_back(var);
}

void X86MiniJavaFrame::AddLocal( const SymbolTable::VariableInfo& name) {
    IAccess* var = new InFrameAccess(RT_Formal, TypeSize(name.GetType().GetType()), localTopPointer );
    localAccess.insert(std::make_pair(name.GetName(), std::unique_ptr<IAccess>(var)));
    localTopPointer += TypeSize(name.GetType().GetType());
}

void X86MiniJavaFrame::AddAddressExit() {
    IAccess* var = createFormal(T_RecordsType::RT_AddressExit, typeSpec->ReferenceSize());
    formalAccess.insert(std::make_pair(StringSymbol::GetIntern(EXIT_ADDRESS_NAME),
                                       std::unique_ptr<IAccess>(var)));
    this->addressExitIndex = formalList.size();
    formalList.push_back(var);
}

void X86MiniJavaFrame::AddAddressReturnValue(SymbolTable::T_VariableType type) {
    IAccess* var = createFormal(T_RecordsType::RT_AddressReturnValue, typeSpec->ReferenceSize());
    formalAccess.insert(std::make_pair(StringSymbol::GetIntern(RETURN_ADDRESS_NAME),
                                       std::unique_ptr<IAccess>(var)));
    this->addressReturnValueIndex = formalList.size();
    formalList.push_back(var);
}

int X86MiniJavaFrame::FormalsCount() const {
    return formalList.size();
}

const IAccess* X86MiniJavaFrame::Formal( int index ) const {
    return formalList[index];
}

const IAccess* X86MiniJavaFrame::FindLocalOrFormal( const StringSymbol* name ) const {
    auto res = localAccess.find(name);
    if( res == localAccess.end() ) {
        res = formalAccess.find(name);
        if(res == formalAccess.end()) {
            return nullptr;
        }
    }
    return res->second.get();
}

const IAccess* X86MiniJavaFrame::ExitAddress() const
{
    return formalList[addressExitIndex];
}

const IAccess* X86MiniJavaFrame::ReturnAddress() const
{
    return formalList[addressReturnValueIndex];
}

const int X86MiniJavaFrame::FormalSize(int index) const {
    return formalList[index]->GetSize();
}

const int X86MiniJavaFrame::FormalSize(const StringSymbol* name) const {
    auto res = formalAccess.find(name);
    assert(res != formalAccess.end());
    return res->second->GetSize();
}

const SymbolTable::TypeInfo X86MiniJavaFrame::WordType() const
{
    return SymbolTable::TypeInfo(SymbolTable::VT_Int);
}

const TempAddress X86MiniJavaFrame::FP() const {
    return TempAddress(formalTopPointer);
}

const TempAddress X86MiniJavaFrame::SP() const
{
    return TempAddress(FP().GetAddress() + localTopPointer);
}

IAccess* X86MiniJavaFrame::createFormal(T_RecordsType type, int size)
{
    if(formalList.size() < MAX_IN_REG) {
        return new InRegAccess(type, size, formalList.size());
    } else {
        IAccess* access = new InFrameAccess(type, size, formalTopPointer);
        formalTopPointer += size;
        return access;
    }
}

}

