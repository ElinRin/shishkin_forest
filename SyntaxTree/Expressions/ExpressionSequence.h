#ifndef EXPRESSIONSEQUENCE_H
#define EXPRESSIONSEQUENCE_H
#include "common.h"
#include <assert.h>
#include <list>

#include "Expression.h"

struct ExpressionSequence : public ITreeNode {
    std::list<IExpression*> Sequence;

    ExpressionSequence(Coordinates& coords, IExpression* expression) :
        coords(coords),
        Sequence(1, expression)
    {
        assert(expression);
    }
    ExpressionSequence(Coordinates& coords, const ExpressionSequence&& sequence,
                       IExpression* expression) :
        coords(coords),
        Sequence(std::move(sequence.Sequence))
    {
        assert(expression);
        Sequence.emplace_back(expression);
    }

    ~ExpressionSequence() {
        for(auto exp: Sequence) {
            delete exp;
        }
    }

    ACCEPT_VISITOR
};

#endif // EXPRESSIONSEQUENCE_H
