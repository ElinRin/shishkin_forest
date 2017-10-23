#ifndef BRACEDSEQUENCESTATEMENT_H
#define BRACEDSEQUENCESTATEMENT_H
#include "common.h"

#include "Statement.h"

struct BraceSequenceStatement : public IStatement
{
    const Sequence<const IStatement>* BracedSequence;

    BraceSequenceStatement(Coordinates coords,
                           const Sequence<const IStatement>* sequence) :
        coords(coords),
        BracedSequence(sequence)
    {}

    ~BraceSequenceStatement() {
        delete BracedSequence;
    }

    ACCEPT_VISITOR
};


#endif
