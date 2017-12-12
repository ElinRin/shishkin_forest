#pragma once

#include <memory>

#include "Access.h"
#include "Temp.h"

namespace ActivationRecords {

class InRegAccess : public IAccess {
public: 
    InRegAccess(T_RecordsType _type, int _size, int regNumber);

    virtual const int GetSize() const override { return size; }
    virtual const T_RecordsType GetRecordType() override { return type; }
    virtual const int GetRegNumber() const { return regNumber; }
    virtual void print(Temp fp) const override;

private:
    T_RecordsType type;
    int size;
    int regNumber;
};

}
