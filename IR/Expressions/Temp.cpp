#include "Temp.h"

namespace IR {

namespace {
 static int counter = 0;
}

Temp::Temp(std::string name, const Coords& coords, int uniqueId) :
    Id(counter++),
    localId(0),
    name(name),
    info(AI_Name),
    unique(uniqueId != -1),
    coords(coords)
{
    if(unique) {
        Id = uniqueId;
        localId = uniqueId;
    }
}

Temp::Temp(int localId, const Coords& coords) :
    Id(counter++),
    localId(localId),
    name(""),
    info(AI_Id),
    unique(false),
    coords(coords)
{
}

Temp::Temp(const Temp &temp):
    Id(temp.Id),
    localId(temp.localId),
    name(temp.name),
    info(temp.info),
    unique(temp.unique),
    coords(temp.coords)
{
}

}
