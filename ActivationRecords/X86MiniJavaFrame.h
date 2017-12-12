#pragma once

#include "Frame.h"
#include "Access.h"
#include <unordered_map> 
#include <vector>
#include <string>

namespace ActivationRecords {

class X86MiniJavaFrame : public IFrame {
    /*
     * LocalAddress = access address + FP
     * FormalAddress = access address or regIndex
     */
public:
    static const int WORD_SIZE = 4;

    virtual void AddFormal( const SymbolTable::VariableInfo& name) override;
    virtual void AddLocal( const SymbolTable::VariableInfo& name) override;
    virtual void AddAddressExit() override;
    virtual void AddAddressReturnValue(SymbolTable::T_VariableType type) override;
    virtual int FormalsCount() const override;
    virtual const IAccess* Formal( int index ) const override;
    virtual const IAccess* FindLocalOrFormal(const StringSymbol* name ) const override;
    virtual const IAccess* ExitAddress() const override;
    virtual const IAccess* ReturnAddress() const override;
    const int FormalSize(int index) const;
    const int FormalSize(const StringSymbol* name) const;
    virtual const SymbolTable::TypeInfo WordType() const override;
    virtual const Temp FP() const override;
    virtual const Temp SP() const override;

private:
    std::vector<IAccess*> formalList;
    std::unordered_map<std::string, IAccess*> formalAccess;
    std::unordered_map<std::string, IAccess*> localAccess;
    Temp framePointer;
    Temp stackPointer;
    int addressExitIndex;
    int addressReturnValueIndex;
    int formalTopPointer = 0;
    int localTopPointer = 0;

    IAccess* createFormal(T_RecordsType type, int size);
};

}
