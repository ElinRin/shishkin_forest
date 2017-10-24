#ifndef MAINCLASS_H
#define MAINCLASS_H
#include "common.h"

#include "Id.h"
#include "Statements/Statement.h"

struct MainClass : public ITreeNode
{
    const Id* ClassName;
    const IStatement* MainStatement;

    MainClass(Coordinates coords, const Id* className,
                const IStatement* mainStatement) :
        coords(coords),
        ClassName(className),
        MainStatement(mainStatement)
    {}

    ~MainClass() {
        delete ClassName;
        delete MainStatement;
    }

    ACCEPT_VISITOR
};

#endif
