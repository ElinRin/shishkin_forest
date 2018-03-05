#pragma once

#include "common.h"
#include "ClassStruct.h"
#include "TypeSpec.h"

namespace SymbolTable {

class X86MiniJavaClassStruct : public IClassStruct {
public:
    X86MiniJavaClassStruct();

    virtual void AddClassName(const StringSymbol* className) override { this->className = className; }
    virtual void AddToVtable(const MethodInfo* info) override;
    virtual void AddField(const VariableInfo* info) override;

    virtual const std::vector<const MethodInfo*>& GetVTable() const override { return vtableEntries; }
    virtual const StringSymbol* GetTableName() const override;

    virtual IR::IExp* GetFieldFrom(const StringSymbol* fieldName,
                                         IR::IExp* base, const Position& position=Position()) const override;
    virtual IR::IExp* GetVirtualMethodAddress(const StringSymbol* methodName,
                                            IR::IExp* base, const Position& position=Position()) const override;
    virtual IR::IExp* AllocateNew(const Position& position=Position()) const override;

private:
    // Vtable
    std::vector<const MethodInfo*> vtableEntries;
    // Method name -> vtableEntries[id]
    std::unordered_map<const StringSymbol*, int> vTableIndices;
    // Field name -> fields[id]
    std::unordered_map<const StringSymbol*, int> fieldsOffsets;
    std::unique_ptr<const ActivationRecords::ITypeSpec> typeSpec;
    int totalFieldsSize;
    const StringSymbol* className;
};

}
