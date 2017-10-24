#ifndef NEWOBJECTEXPRESSION_H
#define NEWOBJECTEXPRESSION_H
#include "common.h"

#include "Expression.h"
#include "Id.h"

namespace AST {

struct NewObjectExpression : public IExpression
{
    std::unique_ptr<const Id> ObjectId;

    NewObjectExpression(Coordinates coords, const Id* objectId) :
        coords(coords),
        ObjectId(objectId) {}

    ACCEPT_VISITOR
};

}

#endif // NEWOBJECTEXPRESSION_H
