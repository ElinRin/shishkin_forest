#pragma once

#include "Exp.h"

namespace IR {

namespace ArrayStruct {

// Array format: [Length, 0, 1, ..., n - 1]

const int ElementsOffset = 1;

IExp* GetElement(IExp* base, IExp* elementNumber, const IR::Coords& coords=Coords());
IExp* GetLength(IExp* base, const IR::Coords& coords=Coords());
IStm* AddReference(IExp* base);
IStm* RemoveReference(IExp* base);

}

}


