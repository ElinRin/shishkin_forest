#ifndef TYPE_H
#define TYPE_H
#include "common.h"

#include "TreeNode.h"

namespace AST {

enum T_Type {
    T_Int,
    T_Boolean,
    T_IntArray,
    T_UserType
};

struct Type : public ITreeNode
{
    const T_Type TypeEnum;
    // Nullable
    const Id* TypeName;

    Type(Coordinates coords,
         const T_Type typeEnum,
         const Id* name = nullptr) :
        TypeEnum(typeEnum),
        TypeName(name),
        coords(coords)
    {}

    ~Type() {
        if(TypeName != nullptr) {
            delete TypeName;
        }
    }

    std::string ToString() const {
        switch (TypeEnum) {
        case T_Int:
            return "int";
        case T_Boolean:
            return "boolean";
        case T_IntArray:
            return "int[]";
        case T_UserType:
            return this->TypeName->Name;
        default:
            assert(false);
        }
    }

    ACCEPT_VISITOR
};

}

#endif
