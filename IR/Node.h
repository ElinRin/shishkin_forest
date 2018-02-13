#pragma once
#include "common.h"

#include "Position.h"

#define ACCEPT_IR_VISITOR public: virtual void AcceptVisitor(IR::IIRVisitor* visitor) const override; \
                        private: const IR::Coords coords; \
                        public: const IR::Coords& GetCoords() const { return coords; }

namespace IR {

typedef SymbolTable::Position Coords;

interface IIRVisitor;

interface INode
{
    virtual ~INode() {}
    virtual void AcceptVisitor(IIRVisitor* visitor) const = 0;
};

}
