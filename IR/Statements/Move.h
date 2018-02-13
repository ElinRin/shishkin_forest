#pragma once

#include "IExp.h"
#include "IStm.h"

namespace IR {

class Move: public IStm {
public:
    std::unique_ptr<const IExp> Destination;
    std::unique_ptr<const IExp> Source;

    Move(const IExp* destination, const IExp* source, const Coords& coords=Coords()):
        Destination(destination), Source(source), coords(coords)
    {}

    ACCEPT_IR_VISITOR
};

}
