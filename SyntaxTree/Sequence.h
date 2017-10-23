#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "common.h"

#include <list>

#include "TreeNode.h"

template< typename T>
struct Sequence : public ITreeNode
{
    std::list<const T*> SequenceList;
    Sequence(Coordinates coords) :
        coords(coords)
    {}

    Sequence(Coordinates coords,
                     const T* appendix) :
        coords(coords),
        SequenceList(1, appendix)
    {
    }

    Sequence(Coordinates &coords,
                     Sequence<T>&& sequence,
                     const T* appendix):
        coords(coords),
        SequenceList(std::move(sequence.SequenceList))
    {
        SequenceList.emplace_back(appendix);
    }

    ~Sequence() {
        for(auto element : SequenceList) {
            delete element;
        }
    }

    ACCEPT_VISITOR
};

#endif
