#include "ClassStruct.h"

#include <assert.h>

#include "IRNodeTypes.h"
#include "NameConventions.h"

/*namespace SymbolTable {

const IR::IExp* ClassStruct::GetFieldFrom(const StringSymbol* fieldName,
                                          const IR::IExp* base) const
{
    assert(fieldsOffsets.find(fieldName) != fieldsOffsets.end());
    int wordSize = sampleFrameType->TypeSize(sampleFrameType->WordType().GetType());
    return new IR::Mem(new IR::Binop(IR::Binop::TB_PLUS, base,
                                     new IR::Const(wordSize*(1 + vtableEntries.size()) + fieldsOffsets.at(fieldName))));
}

const IR::IExp *ClassStruct::GetVirtualMethodAddress(const StringSymbol* methodName, const IR::IExp *base) const
{
    assert(vTableIndices.find(methodName) != vTableIndices.end());
    int wordSize = sampleFrameType->TypeSize(sampleFrameType->WordType().GetType());
    return new IR::Mem(new IR::Binop(IR::Binop::TB_PLUS, base,
                                                   new IR::Const(wordSize*(1 + vTableIndices.at(methodName)))));
}

const IR::IExp *ClassStruct::AllocateNew() const
{
    int wordSize = sampleFrameType->TypeSize(sampleFrameType->WordType().GetType());
    IR::ExpList* allocArg = new IR::ExpList(new IR::Const(totalFieldsSize +
                                                          wordSize*(vtableEntries.size() + 1)));
    const int baseAddressId = 0;
    IR::StmList* prepareActions = new IR::StmList(new IR::Move(new IR::Temp(baseAddressId),
                                                               new IR::Call(new IR::Name(NameConventions::MallocName),
                                                                            allocArg)));

    prepareActions = new IR::StmList(new IR::Move(new IR::Mem(new IR::Temp(baseAddressId)),
                                                  new IR::Const((vtableEntries.size() + 1) * wordSize)), prepareActions);
    int offset = 0;
    for(int i = 0; i < vtableEntries.size(); ++i) {
        prepareActions = new IR::StmList(new IR::Move(
                                                new IR::Binop(IR::Binop::TB_PLUS,
                                                              new IR::Mem(new IR::Temp(baseAddressId)),
                                                              new IR::Const(offset+wordSize) ),
                                                new IR::Name(vtableEntries[i]->GetFullName()->GetString())),
                                         prepareActions);
        offset += wordSize;
    }
    for(auto offset = fieldsOffsets.begin(); offset != fieldsOffsets.end(); ++offset) {
        prepareActions = new IR::StmList(new IR::Move(
                                                new IR::Binop(IR::Binop::TB_PLUS,
                                                              new IR::Mem(new IR::Temp(baseAddressId)),
                                                              new IR::Const(offset->second + wordSize*(vtableEntries.size() + 1))),
                                                new IR::Const(0)),
                                         prepareActions);
    }
    return new IR::Eseq(prepareActions, new IR::Temp(baseAddressId));
}

}
*/
