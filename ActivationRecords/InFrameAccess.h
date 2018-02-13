#pragma once

#include "Access.h"
#include "Frame.h"

namespace ActivationRecords {

class InFrameAccess : public IAccess {
public: 
    InFrameAccess( T_RecordsType _type, int _size, int offset );
    virtual const int GetSize() const override { return size; }
    virtual const T_RecordsType GetRecordType() override { return recordType; }
    virtual const IR::IExp* GetExp(const IR::Temp* fp, const SymbolTable::Position& position) const override;
    virtual void print(TempAddress fp) const override;
    const TempAddress& Offset() const;
private:
    const int size;
    const TempAddress address;
    const T_RecordsType recordType;
};

}
