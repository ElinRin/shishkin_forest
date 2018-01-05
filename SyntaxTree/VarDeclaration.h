#ifndef VARDECLARATION_H
#define VARDECLARATION_H
#include "common.h"

#include "Type.h"

namespace AST {

struct VarDeclaration : public ITreeNode
{
    std::unique_ptr<const Type> VarType;
    std::unique_ptr<const Id> Identifier;

    VarDeclaration(Coordinates coords,
                   const Type* varType,
                   const Id* identifier) :
        VarType(varType),
        Identifier(identifier),
        coords(coords)
    {}

    ACCEPT_VISITOR
};

}

#endif
