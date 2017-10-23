#ifndef VARDECLARATION_H
#define VARDECLARATION_H
#include "common.h"

#include "Type.h"

struct VarDeclaration : public ITreeNode
{
    const Type* VarType;
    const Id* Identifier;

    VarDeclaration(Coordinates coords,
                   const Type* varType,
                   const Id* identifier) :
        coords(coords),
        VarType(varType),
        Identifier(identifier)
    {}

    ~VarDeclaration() {
        delete VarType;
        delete Identifier;
    }

    ACCEPT_VISITOR
};

#endif
