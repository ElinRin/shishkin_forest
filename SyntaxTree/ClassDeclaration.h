#ifndef CLASSDECLARATION_H
#define CLASSDECLARATION_H
#include "common.h"

#include "Sequence.h"
#include "VarDeclaration.h"
#include "MethodDeclaration.h"

struct ClassDeclaration : public ITreeNode
{
    const Id* ClassName;

    //Nullable
    const Id* SuperName;
    const Sequence<const VarDeclaration>* VarDeclarations;
    const Sequence<const MethodDeclaration>* MethodDeclarations;

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

    ~ClassDeclaration() {
        delete ClassName;
        if(SuperName != nullptr) {
            delete SuperName;
        }
        if(VarDeclarations!= nullptr) {
            delete VarDeclarations;
        }
        if(MethodDeclarations != nullptr) {
            delete MethodDeclarations;
        }
    }

    ACCEPT_VISITOR
};

#endif
