#ifndef METHODDECLARATION_H
#define METHODDECLARATION_H
#include "common.h"

#include <assert.h>

#include "Qualifier.h"
#include "VarDeclaration.h"
#include "Statements/ReturnStatement.h"

namespace AST {

struct MethodDeclaration : public ITreeNode
{
    std::unique_ptr<const Qualifier> QualifierType;
    std::unique_ptr<const Type> ReturnType;
    std::unique_ptr<const Id> MethodName;
    std::unique_ptr<const ReturnStatement> StatementToReturn;

    //Nullable
    std::unique_ptr<const Sequence<const VarDeclaration>> Arguments;
    std::unique_ptr<const Sequence<const VarDeclaration>> VarDeclarations;
    std::unique_ptr<const Sequence<const IStatement>> Statements;

    MethodDeclaration(Coordinates coords,
                      const Qualifier* qualifier,
                      const Type* returnType,
                      const Id* methodName,
                      const Sequence<const VarDeclaration>* argumentSequence,
                      const Sequence<const VarDeclaration>* varDeclarations,
                      const Sequence<const IStatement>* statements,
                      const ReturnStatement* returnStatement) :
        coords(coords),
        QualifierType(qualifier),
        ReturnType(returnType),
        MethodName(methodName),
        StatementToReturn(returnStatement),
        Arguments(argumentSequence),
        VarDeclarations(varDeclarations),
        Statements(statements)
    {
        assert(argumentSequence);
    }

    ACCEPT_VISITOR
};

}

#endif
