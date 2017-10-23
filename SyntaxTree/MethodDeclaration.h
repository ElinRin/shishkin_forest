#include METHODDECLARATION_H
#define METHODDECLARATION_H
#include "common.h"

#include "Qualifier.h"
#include "VarDeclaration.h"

struct MethodDeclartaion : public ITreeNode
{
    const Qualifier* QualifierType;
    const Type* ReturnType;
    const Id* MethodName;
    const ReturnStatement* StatementToReturn;
    const ArgumentSequence* Arguments;

    MethodDeclartaion(Coordinates& coords,
                      const Qualifier* qualifier,
                      const Type* returnType,
                      const Id* methodName,
                      const ReturnStatement* returnStatement,
                      const ArgumentSequence* argumentSequence) :
        coords(coords),
        QualifierType(qualifier),
        ReturnType(returnType),
        MethodName(methodName),
        StatementToReturn(returnStatement),
        Arguments(argumentSequence)
    {
        assert(argumentSequence);
    }

    ACCEPT_VISITOR
};

#endif
