#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "common.h"

#include <list>

#include "TreeNode.h"

template< typename T>
struct Sequence : public ITreeNode
{
    std::list<const T*> Sequence;
    Sequence(Coordinates& coords) :
        coords(coords)
    {}

    Sequence(Coordinates& coords,
                     const T* appendix) :
        coords(coords),
        Sequence(1, appendix)
    {
    }

    Sequence(Coordinates &coords,
                     Sequence<T>&& sequence,
                     const T* appendix):
        coords(coords),
        Sequence(std::move(sequence.Sequence))
    {
        Sequence.emplace_back(appendix);
    }

    ~Sequence() {
        for(auto element : Sequence) {
            delete element;
        }
    }

    ACCEPT_VISITOR
};

#endif
