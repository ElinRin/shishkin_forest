#ifndef METHODDECLARATION_H
#define METHODDECLARATION_H
#include "common.h"

#include <assert.h>

#include "Qualifier.h"
#include "VarDeclaration.h"
#include "Statements/ReturnStatement.h"

struct MethodDeclaration : public ITreeNode
{
    const Qualifier* QualifierType;
    const Type* ReturnType;
    const Id* MethodName;
    const ReturnStatement* StatementToReturn;

    //Nullable
    const Sequence<VarDeclaration>* Arguments;
    const Sequence<IStatement>* Statements;

    MethodDeclaration(Coordinates coords,
                      const Qualifier* qualifier,
                      const Type* returnType,
                      const Id* methodName,
                      const ReturnStatement* returnStatement,
                      const Sequence<VarDeclaration>* argumentSequence,
                      const Sequence<IStatement>* statements) :
        coords(coords),
        QualifierType(qualifier),
        ReturnType(returnType),
        MethodName(methodName),
        StatementToReturn(returnStatement),
        Arguments(argumentSequence),
        Statements(statements)
    {
        assert(argumentSequence);
    }

    ~MethodDeclaration() {
        delete QualifierType;
        delete ReturnType;
        delete MethodName;
        delete StatementToReturn;
        if(Arguments != nullptr) {
            delete Arguments;
        }
        if(Statements != nullptr) {
            delete Statements;
        }
    }

    ACCEPT_VISITOR
};

#endif
