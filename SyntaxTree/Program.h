#ifndef PROGRAM_H
#define PROGRAM_H
#include "common.h"

#include "MainClass.h"
#include "ClassDeclaration.h"

namespace AST {

struct Program : public ITreeNode
{
    std::unique_ptr<const MainClass> Main;
    std::unique_ptr<const Sequence<const ClassDeclaration>> Classes;

    Program(Coordinates coords, const MainClass* main,
            const Sequence<const ClassDeclaration>* classes) :
        coords(coords),
        Main(main),
        Classes(classes)
    {}

    ACCEPT_VISITOR
};

}

#endif
