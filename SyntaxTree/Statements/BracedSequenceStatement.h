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
        BracedSequence(sequence),
        coords(coords)
    {}

    ACCEPT_AST_VISITOR
};

}

#endif
