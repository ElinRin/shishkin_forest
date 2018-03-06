#pragma once

#include <memory>

#include "Access.h"
#include "TempAddress.h"
#include "IRNodeTypes.h"

namespace ActivationRecords {

class InRegAccess : public IAccess {
public: 
    InRegAccess(T_RecordsType _type, int _size, const std::string& name);
    InRegAccess(T_RecordsType _type, int _size, int id);
    InRegAccess(const InRegAccess& other);

    virtual const int GetSize() const override { return size; }
    virtual const T_RecordsType GetRecordType() override { return type; }
    virtual void print(TempAddress fp) const override;
    virtual IR::IExp* GetExp(IR::Temp* fp, const SymbolTable::Position& position) const override;
    const std::string& GetName() const { return name; }
    const int GetId() const { return id; }

private:
    T_RecordsType type;
    int size;
    int id;
    const std::string name;
    std::unique_ptr<IR::Temp> temp;
};

}
