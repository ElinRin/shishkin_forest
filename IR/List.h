#pragma once

#include <memory>
#include <assert.h>
#include <IExp.h>
#include <IStm.h>

namespace IR {

template<typename T>
class List : public T{
public:
    std::unique_ptr<const T> Head;
    std::unique_ptr<const T> Tail;

    List(const T* head=nullptr, const T* tail=nullptr, const Coords& coords=Coords()) :
        Head(head),
        Tail(tail),
        coords(coords)
    {
    }

    ACCEPT_IR_VISITOR
};

typedef List<IExp> ExpList;
typedef List<IStm> StmList;

}
