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

const IR::IExp* X86MiniJavaClassStruct::GetFieldFrom(const StringSymbol* fieldName,
                                          const IR::IExp* base, const Position& position) const
{
    assert(fieldsOffsets.find(fieldName) != fieldsOffsets.end());
    int wordSize = typeSpec->WordSize();
    return new IR::Mem(new IR::Binop(IR::Binop::TB_PLUS, base,
                                     new IR::Const(wordSize*(1 + vtableEntries.size()) + fieldsOffsets.at(fieldName), position),
                                     position), position);
}

const IR::IExp* X86MiniJavaClassStruct::GetVirtualMethodAddress(const StringSymbol* methodName, const IR::IExp *base, const Position &position) const
{
    assert(vTableIndices.find(methodName) != vTableIndices.end());
    int wordSize = typeSpec->WordSize();
    return new IR::Mem(new IR::Binop(IR::Binop::TB_PLUS, base,
                                                   new IR::Const(wordSize*(1 + vTableIndices.at(methodName)))));
}

const IR::IExp* X86MiniJavaClassStruct::AllocateNew(const Position& position) const
{
    int wordSize = typeSpec->WordSize();
    IR::ExpList* allocArg = new IR::ExpList(new IR::Const(totalFieldsSize +
                                                          wordSize*(vtableEntries.size() + 1), position), nullptr, position);
    const int baseAddressId = 0;
    IR::StmList* prepareActions = new IR::StmList(new IR::Move(new IR::Temp(baseAddressId, position),
                                                               new IR::Call(new IR::Name(NameConventions::MallocName, position),
                                                                            allocArg, position), position), nullptr, position);

    prepareActions = new IR::StmList(new IR::Move(new IR::Mem(new IR::Temp(baseAddressId, position), position),
                                                  new IR::Const((vtableEntries.size() + 1) * wordSize, position), position), prepareActions, position);
    int offset = 0;
    for(int i = 0; i < vtableEntries.size(); ++i) {
        prepareActions = new IR::StmList(new IR::Move(
                                                new IR::Binop(IR::Binop::TB_PLUS,
                                                              new IR::Mem(new IR::Temp(baseAddressId, position), position),
                                                              new IR::Const(offset+wordSize, position), position ),
                                                new IR::Name(vtableEntries[i]->GetFullName()->GetString(), position), position),
                                         prepareActions, position);
        offset += wordSize;
    }
    for(auto offset = fieldsOffsets.begin(); offset != fieldsOffsets.end(); ++offset) {
        prepareActions = new IR::StmList(new IR::Move(
                                                new IR::Binop(IR::Binop::TB_PLUS,
                                                              new IR::Mem(new IR::Temp(baseAddressId, position), position),
                                                              new IR::Const(offset->second + wordSize*(vtableEntries.size() + 1), position), position),
                                                new IR::Const(0, position), position),
                                         prepareActions, position);
    }
    return new IR::Eseq(prepareActions, new IR::Temp(baseAddressId, position), position);
}

}
