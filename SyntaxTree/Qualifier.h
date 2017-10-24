#ifndef QUALIFIER_H
#define QUALIFIER_H
#include "common.h"

#include "TreeNode.h"

namespace AST {

enum T_Qualifier {
    Q_Public,
    Q_Private
};

struct Qualifier : public ITreeNode
{
    const T_Qualifier QualifierEnum;

    Qualifier(Coordinates coords, T_Qualifier qualifierEnum) :
        coords(coords),
        QualifierEnum(qualifierEnum)
    {}

    ACCEPT_VISITOR
};

}

#endif
