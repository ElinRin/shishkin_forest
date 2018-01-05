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
    const int Id;

    Temp(std::string& name);
    Temp(int localId);
private:
    const int localId;
    std::string name;
    T_AdditionalInfo info;
};

}
