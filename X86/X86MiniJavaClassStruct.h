#pragma once

#include "common.h"
#include "ClassStruct.h"
#include "TypeSpec.h"

namespace SymbolTable {

class X86MiniJavaClassStruct : public IClassStruct {
public:
    X86MiniJavaClassStruct();

    virtual void AddToVtable(const MethodInfo* info) override;
    virtual void AddField(const VariableInfo* info) override;

    virtual const IR::IExp* GetFieldFrom(const StringSymbol* fieldName,
                                         const IR::IExp* base, const Position& position=Position()) const override;
    virtual const IR::IExp* GetVirtualMethodAddress(const StringSymbol* methodName,
                                            const IR::IExp* base, const Position& position=Position()) const override;
    virtual const IR::IExp* AllocateNew(const Position& position=Position()) const override;

private:
    // Vtable
    std::vector<const MethodInfo*> vtableEntries;
    // Method name -> vtableEntries[id]
    std::unordered_map<const StringSymbol*, int> vTableIndices;
    // Field name -> fields[id]
    std::unordered_map<const StringSymbol*, int> fieldsOffsets;
    std::unique_ptr<const ActivationRecords::ITypeSpec> typeSpec;
    int totalFieldsSize;

};

}