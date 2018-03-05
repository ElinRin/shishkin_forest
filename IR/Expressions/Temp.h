#pragma once

#include "IExp.h"
#include "TempAddress.h"
#include <memory>
#include <string>

namespace AR = ActivationRecords;

namespace IR {

class Temp : public IExp {
public:
    enum T_AdditionalInfo{
      AI_Id,
      AI_Name
    };
    static const int TempHolderLocalId = 2;
    const int Id;

    //explicit Temp(std::string name);
    explicit Temp(std::string name, const Coords coords=Coords());
    //Bexplicit Temp(int localId);
    explicit Temp(int localId, const Coords coords=Coords());
    Temp(const Temp& temp);

    const T_AdditionalInfo Info() const { return info; }
    const std::string Name() const { return name; }
    int LocalId() const { return localId; }

    virtual bool IsCommutative() const override { return true; }

private:
    const int localId;
    std::string name;
    T_AdditionalInfo info;

    ACCEPT_IR_VISITOR
};

}
