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
        ClassName(className),
        MainStatement(mainStatement),
        coords(coords)
    {}

    ACCEPT_AST_VISITOR
};

}

#endif
