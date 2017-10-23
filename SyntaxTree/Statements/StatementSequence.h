#ifndef STATEMENTSEQUENCE_H
#define STATEMENTSEQUENCE_H
#include "common.h"
#include <assert.h>
#include <list>


#include "TreeNode.h"

struct StatementSequence : public ITreeNode
{
    std::list<IStatement*> Sequence;
    StatementSequence(Coordinates coords, IStatement* statement) :
        coords(coords),
        Sequence(1, statement)
    {
        assert(statement);
    }

    StatementSequence(Coordinates coords, StatementSequence&& sequence,
                      IStatement* statement) :
        coords(coords),
        Sequence(std::move(sequence.Sequence))
    {
        assert(statement);
        Sequence.emplace_back(statement);
    }

    ~StatementSequence() {
        for(auto statement : Sequence) {
            delete statement;
        }
    }

    ACCEPT_VISITOR
};


#endif
