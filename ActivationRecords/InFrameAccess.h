#pragma once

#include "Access.h"
#include "Frame.h"

namespace ActivationRecords {

class InFrameAccess : public IAccess {
public: 
    InFrameAccess( T_RecordsType _type, int _size, int offset );
    virtual const int GetSize() const override { return size; }
    virtual const T_RecordsType GetRecordType() override { return recordType; }
    const Temp& Offset() const;
    virtual void print(Temp fp) const override;
private:
    const int size;
    const Temp address;
    const T_RecordsType recordType;
};

}
