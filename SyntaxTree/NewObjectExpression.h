#ifndef NEWOBJECTEXPRESSION_H
#define NEWOBJECTEXPRESSION_H
#include "common.h"

#include "Expression.h"
#include "Id.h"

struct NewObjectExpression : public IExpression
{
    const Id* ObjectId;

    NewObjectExpression(const Id* objectId) :
        ObjectId(objectId) {}
    ~NewObjectExpression() { delete ObjectId; }

    ACCEPT_VISITOR
};

#endif // NEWOBJECTEXPRESSION_H
