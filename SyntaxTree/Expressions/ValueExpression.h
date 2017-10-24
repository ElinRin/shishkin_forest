#ifndef VALUEEXPRESSION_H
#define VALUEEXPRESSION_H
#include "common.h"

#include "Expression.h"

namespace AST {

enum T_ValueType {
    VT_Int,
    VT_Boolean
};

struct ValueExpression : public IExpression {
    const T_ValueType ValueType;
    const int Value;

    ValueExpression(Coordinates coords, T_ValueType type,
                    int value) :
        coords(coords),
        ValueType(type),
        Value(value) { }

    ACCEPT_VISITOR
};

}

#endif // VALUEEXPRESSION_H
