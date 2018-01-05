#include "Temp.h"

namespace IR {

namespace {
 static int counter = 0;
}

Temp::Temp(std::string& name) :
    Id(counter++),
    localId(0),
    name(name),
    info(AI_Name)
{
}

Temp::Temp(int localId) :
    Id(counter++),
    localId(localId),
    name(""),
    info(AI_Id)
{
}

}
