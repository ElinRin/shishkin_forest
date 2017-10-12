#ifndef ID_H
#define ID_H
#include "common.h"

#include <string.h>

#include "TreeNode.h"

struct Id : public ITreeNode {

    const std::string Name;

    Id(const std::string& name) :
        Name(name) {}

    ACCEPT_VISITOR
};

#endif // ID_H
