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
    const Sequence<VarDeclaration>* VarDeclarations;
    const Sequence<MethodDeclaration>* MethodDeclarations;

    ClassDeclaration(Coordinates coords, const Id* className,
                     const Id* superName,
                     const Sequence<VarDeclaration>* varDeclarations,
                     const Sequence<MethodDeclaration>* methodDeclarations) :
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
