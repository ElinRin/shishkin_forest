#ifndef CLASSDECLARATION_H
#define CLASSDECLARATION_H
#include "common.h"

#include "Sequence.h"
#include "VarDeclaration.h"
#include "MethodDeclaration.h"

namespace AST {

struct ClassDeclaration : public ITreeNode
{
    std::unique_ptr<const Id> ClassName;

    //Nullable
    std::unique_ptr<const Id> SuperName;
    std::unique_ptr<const Sequence<const VarDeclaration>> VarDeclarations;
    std::unique_ptr<const Sequence<const MethodDeclaration>> MethodDeclarations;

    ClassDeclaration(Coordinates coords, const Id* className,
                     const Id* superName,
                     const Sequence<const VarDeclaration>* varDeclarations,
                     const Sequence<const MethodDeclaration>* methodDeclarations) :
        coords(coords),
        ClassName(className),
        SuperName(superName),
        VarDeclarations(varDeclarations),
        MethodDeclarations(methodDeclarations)
    {}

    ACCEPT_VISITOR
};

}

#endif
