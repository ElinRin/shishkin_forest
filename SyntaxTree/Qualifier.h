#ifndef QUALIFIER_H
#define QUALIFIER_H
#include "common.h"

#include "TreeNode.h"

enum T_Qualifier {
    S_Public,
    S_Private
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

#endif
