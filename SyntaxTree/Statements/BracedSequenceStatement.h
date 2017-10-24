#ifndef BRACEDSEQUENCESTATEMENT_H
#define BRACEDSEQUENCESTATEMENT_H
#include "common.h"

#include "Statement.h"

namespace AST {

struct BraceSequenceStatement : public IStatement
{
    std::unique_ptr<const Sequence<const IStatement>> BracedSequence;

    BraceSequenceStatement(Coordinates coords,
                           const Sequence<const IStatement>* sequence) :
        coords(coords),
        BracedSequence(sequence)
    {}

    ACCEPT_VISITOR
};

}

#endif
