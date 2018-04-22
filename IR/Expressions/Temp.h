#pragma once

#include "IExp.h"
#include "TempAddress.h"
#include <memory>
#include <string>
#include <vector>

namespace AR = ActivationRecords;

namespace IR {

class Temp : public IExp {
public:
    enum T_AdditionalInfo{
      AI_Id,
      AI_Name
    };
    static const int TempHolderLocalId = 9000;
    int Id;

    //explicit Temp(std::string name);
    explicit Temp(std::string name, const Coords& coords=Coords(), int uniqueId = -1);
    //explicit Temp(int localId);
    explicit Temp(int localId, const Coords& coords=Coords());
    Temp(const Temp& temp);

    const T_AdditionalInfo Info() const { return info; }
    const std::string Name() const { return name; }
    int LocalId() const { return localId; }
    bool IsUnique() const { return unique; }

    virtual bool IsCommutative() const override { return true; }
    virtual bool IsAbsolutelyCommutative() const override { return localId == TempHolderLocalId; }

private:
    int localId;
    std::string name;
    T_AdditionalInfo info;
    bool unique;

    ACCEPT_IR_VISITOR
};

typedef std::vector<const Temp*> TempList;

}
