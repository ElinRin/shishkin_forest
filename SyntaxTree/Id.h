#ifndef ID_H
#define ID_H
#include "common.h"

#include <string.h>

#include "TreeNode.h"

namespace AST {

struct Id : public ITreeNode {

    const std::string Name;

    Id(Coordinates coords, const std::string& name) :
        coords(coords),
        Name(name) {}

    ACCEPT_VISITOR
};

}

#endif // ID_H
