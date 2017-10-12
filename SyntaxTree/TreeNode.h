#ifndef _TREE_NODE_H
#define _TREE_NODE_H
#include "common.h"

#include <vector>
#include <memory>

#include "Visitor.h"

interface IVisitor;

#define ACCEPT_VISITOR virtual void AcceptVisitor(IVisitor *visitor) const override;

interface ITreeNode {
    virtual ~ITreeNode();
    virtual void AcceptVisitor(IVisitor* visitor) const = 0;
};

#endif

