#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "common.h"

#include <list>
#include <assert.h>

#include "TreeNode.h"

namespace AST {

template< typename T>
struct Sequence : public ITreeNode
{
    std::list<std::unique_ptr<const T>> SequenceList;

    Sequence(Coordinates coords) :
        coords(coords)
    {}

    Sequence(Coordinates coords,
                     const T* appendix) :
        coords(coords)
    {
        assert(appendix);
        SequenceList.push_back(std::move(std::unique_ptr<const T>(appendix)));
    }

    Sequence(Coordinates coords,
                     Sequence<T>* sequence,
                     const T* appendix):
        coords(coords),
        SequenceList(std::move(std::unique_ptr<Sequence<const T>>(sequence)->SequenceList)) // FML
    {
        assert(appendix);
        SequenceList.emplace_back(appendix);
    }

    ACCEPT_VISITOR
};

}

#endif
