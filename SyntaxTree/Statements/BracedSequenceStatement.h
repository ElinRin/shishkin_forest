#ifndef BRACEDSEQUENCESTATEMENT_H
#define BRACEDSEQUENCESTATEMENT_H
#include "common.h"

#include "StatementSequence.h"

struct BraceSequenceStatement : public IStatement
{
    StatementSequence* Sequence;

    BraceSequenceStatement(Coordinates& coords, const StatementSequence* sequence) :
        coords(coords),
        Sequence(sequence)
    {}

    ~BraceSequenceStatement() {
        delete Sequence;
    }

    ACCEPT_VISITOR
};


#endif
