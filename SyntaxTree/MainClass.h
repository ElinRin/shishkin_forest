#ifndef MAINCLASS_H
#define MAINCLASS_H
#include "common.h"

#include "Id.h"
#include "Statements/Statement.h"

namespace AST {

struct MainClass : public ITreeNode
{
    std::unique_ptr<const Id> ClassName;
    std::unique_ptr<const IStatement> MainStatement;

    MainClass(Coordinates coords, const Id* className,
                const IStatement* mainStatement) :
        coords(coords),
        ClassName(className),
        MainStatement(mainStatement)
    {}

    ACCEPT_VISITOR
};

}

#endif
