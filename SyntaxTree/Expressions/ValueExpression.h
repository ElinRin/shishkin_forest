#ifndef VALUEEXPRESSION_H
#define VALUEEXPRESSION_H
#include "common.h"

#include "Expression.h"

enum T_Value_type {
    VT_Int,
    VT_Boolean
};

struct ValueExpression : public IExpression {
    const T_Value_type ValueType;
    const int Value;

    ValueExpression(Coordinates coords, T_Value_type type,
                    int value) :
        coords(coords),
        ValueType(type),
        Value(value) { }

    ACCEPT_VISITOR
};
#endif // VALUEEXPRESSION_H
