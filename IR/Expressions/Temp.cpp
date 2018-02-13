#include "Temp.h"

namespace IR {

namespace {
 static int counter = 0;
}

Temp::Temp(std::string name, const Coords coords) :
    Id(counter++),
    localId(0),
    name(name),
    info(AI_Name),
    coords(coords)
{
}

Temp::Temp(int localId, const Coords coords) :
    Id(counter++),
    localId(localId),
    name(""),
    info(AI_Id),
    coords(coords)
{
}

Temp::Temp(const Temp &temp):
    Id(temp.Id),
    localId(temp.localId),
    name(temp.name),
    info(temp.info),
    coords(temp.coords)
{
}

}
