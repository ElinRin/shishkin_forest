#ifndef VALUEEXPRESSION_H
#define VALUEEXPRESSION_H
#include "common.h"

#include "Expression.h"

enum VALUE_TYPE {
    VT_INT,
    VT_BOOLEAN
};

struct ValueExpression : public IExpression {
    const VALUE_TYPE ValueType;
    const int Value;

    ValueExpression(VALUE_TYPE type, int value) :
        ValueType(type),
        Value(value) { }

    ACCEPT_VISITOR
};
#endif // VALUEEXPRESSION_H
