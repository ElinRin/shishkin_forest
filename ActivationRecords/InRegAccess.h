#pragma once

#include <memory>

#include "Access.h"
#include "TempAddress.h"

namespace ActivationRecords {

class InRegAccess : public IAccess {
public: 
    InRegAccess(T_RecordsType _type, int _size, int regNumber);

    virtual const int GetSize() const override { return size; }
    virtual const T_RecordsType GetRecordType() override { return type; }
    virtual void print(TempAddress fp) const override;
    virtual const IR::IExp* GetExp(const IR::Temp* fp) const override;
    virtual const int GetRegNumber() const { return regNumber; }

private:
    T_RecordsType type;
    int size;
    int regNumber;
};

}
