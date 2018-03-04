#include "X86MiniJavaClassStruct.h"

#include <assert.h>

#include "IRNodeTypes.h"
#include "NameConventions.h"

namespace SymbolTable {

X86MiniJavaClassStruct::X86MiniJavaClassStruct() :
    typeSpec(Factory::Create<ActivationRecords::ITypeSpec>()),
    totalFieldsSize(0)
{
}

void X86MiniJavaClassStruct::AddToVtable(const MethodInfo* info)
{
    vTableIndices.insert({ info->GetName(), vtableEntries.size() });
    vtableEntries.push_back(info);
}

void X86MiniJavaClassStruct::AddField(const VariableInfo* info)
{
    fieldsOffsets.insert({info->GetName(), totalFieldsSize});
    totalFieldsSize = fieldsOffsets.size();
}

const StringSymbol* X86MiniJavaClassStruct::GetTableName() const
{
    return StringSymbol::GetIntern(NameConventions::VtablePrefix + className->GetString());
}

const IR::IExp* X86MiniJavaClassStruct::GetFieldFrom(const StringSymbol* fieldName,
                                          const IR::IExp* base, const Position& position) const
{
    assert(fieldsOffsets.find(fieldName) != fieldsOffsets.end());
    int wordSize = typeSpec->WordSize();
    return new IR::Mem(new IR::Binop(IR::Binop::TB_PLUS, base,
                                     new IR::Const(wordSize*(1) + fieldsOffsets.at(fieldName), position),
                                     position), position);
}

const IR::IExp* X86MiniJavaClassStruct::GetVirtualMethodAddress(const StringSymbol* methodName, const IR::IExp *base, const Position &position) const
{
    assert(vTableIndices.find(methodName) != vTableIndices.end());
    int wordSize = typeSpec->WordSize();
    return new IR::Mem(new IR::Binop(IR::Binop::TB_PLUS, new IR::Mem(base, position),
                                                   new IR::Const(wordSize*(1 + vTableIndices.at(methodName)), position),
                                     position), position);
}

const IR::IExp* X86MiniJavaClassStruct::AllocateNew(const Position& position) const
{
    int wordSize = typeSpec->WordSize();
    IR::ExpList* allocArg = new IR::ExpList(new IR::Const(totalFieldsSize +
                                                          wordSize*(vtableEntries.size() + 1), position), nullptr, position);
    const int baseAddressId = 0;
    const IR::Temp* baseAddress = new IR::Temp(baseAddressId, position);
    IR::StmList* prepareActions = new IR::StmList(new IR::Move(baseAddress,
                                                               new IR::Call(new IR::Name(NameConventions::MallocName, position),
                                                                            allocArg, position), position), nullptr, position);

    prepareActions = new IR::StmList(prepareActions, new IR::Move(new IR::Mem(new IR::Temp(*baseAddress), position),
                                                  new IR::Name(GetTableName()->GetString(),position), position), position);
    for(auto offset = fieldsOffsets.begin(); offset != fieldsOffsets.end(); ++offset) {
        prepareActions = new IR::StmList(prepareActions, new IR::Move(
                                                new IR::Binop(IR::Binop::TB_PLUS,
                                                              new IR::Mem(new IR::Temp(*baseAddress), position),
                                                              new IR::Const(offset->second + wordSize*(1), position), position),
                                                new IR::Const(0, position), position),
                                         position);
    }
    return new IR::Eseq(prepareActions, new IR::Temp(*baseAddress), position);
}

}
