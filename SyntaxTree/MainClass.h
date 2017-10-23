#ifndef MAINCLASS_H
#define MAINCLASS_H
#include "common.h"

#include "Id.h"
#include "Statements/Statement.h"

struct MainClass : public ITreeNode
{
    const Id* ClassName;
    const Id* ArgsName;
    const IStatement* MainStatement;

    MainClass(Coordinates coords, const Id* className,
              const Id* argsName, const IStatement* mainStatement) :
        coords(coords),
        ClassName(className),
        ArgsName(argsName),
        MainStatement(mainStatement)
    {}

    ~MainClass() {
        delete ClassName;
        delete ArgsName;
        delete MainStatement;
    }

    ACCEPT_VISITOR
};

#endif
