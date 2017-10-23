#ifndef PROGRAM_H
#define PROGRAM_H
#include "common.h"

#include "MainClass.h"
#include "ClassDeclaration.h"

struct Program : public ITreeNode
{
    const MainClass* Main;
    const Sequence<ClassDeclaration>* Classes;

    Program(Coordinates coords, const MainClass* main,
            const Sequence<ClassDeclaration>* classes) :
        coords(coords),
        Main(main),
        Classes(classes)
    {}

    ~Program() {
        delete Main;
        delete Classes;
    }

    ACCEPT_VISITOR
};

#endif
