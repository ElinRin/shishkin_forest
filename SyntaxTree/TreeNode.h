#ifndef _TREE_NODE_H
#define _TREE_NODE_H
#include "common.h"

#include <vector>
#include <memory>

#include "Visitor.h"

#define ACCEPT_VISITOR virtual void AcceptVisitor(IVisitor *visitor) const override; \
    virtual const Coordinates& Coords() const { return coords; } \
    private: Coordinates coords; public:

namespace AST {

interface IVisitor;

struct Coordinates
{
    int Row;
    int Column;
    Coordinates(int row, int column) :
        Row(row),
        Column(column)
    {}
    Coordinates(const Coordinates& coordinates) :
        Row(coordinates.Row),
        Column(coordinates.Column)
    {}
};

interface ITreeNode {
    virtual const Coordinates& Coords() const = 0;
    virtual ~ITreeNode();
    virtual void AcceptVisitor(IVisitor* visitor) const = 0;
};

}

#endif

