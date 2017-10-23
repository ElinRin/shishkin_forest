#ifndef BRACEDSEQUENCESTATEMENT_H
#define BRACEDSEQUENCESTATEMENT_H
#include "common.h"

#include "Statement.h"

struct BraceSequenceStatement : public IStatement
{
    const Sequence<IStatement>* BracedSequence;

    BraceSequenceStatement(Coordinates coords, const Sequence<IStatement>* sequence) :
        coords(coords),
        BracedSequence(sequence)
    {}

    ~BraceSequenceStatement() {
        delete BracedSequence;
    }

    ACCEPT_VISITOR
};


#endif
