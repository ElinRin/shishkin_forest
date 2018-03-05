#pragma once

#include <memory>
#include <assert.h>
#include <IExp.h>
#include <IStm.h>

namespace IR {

template<typename T>
class List : public T{
public:
    std::unique_ptr<T> Head;
    std::unique_ptr<T> Tail;

    List(T* head=nullptr, T* tail=nullptr, const Coords& coords=Coords()) :
        Head(head),
        Tail(tail),
        coords(coords)
    {
    }

    virtual bool IsCommutative() const { return false; }
    virtual bool IsAbsolutelyCommutative() const { return false; }

    ACCEPT_IR_VISITOR
};

typedef List<IExp> ExpList;
typedef List<IStm> StmList;

}
