#pragma once

#include <memory>
#include <assert.h>
#include <IExp.h>
#include <IStm.h>

namespace IR {

template<typename T>
class List {
public:
    std::unique_ptr<const T> Head;
    std::unique_ptr<List<T>> Tail;

    List(const T* head, List<T>* tail = nullptr) :
        Head(head),
        Tail(tail)
    {
        assert(head != nullptr);
    }
};

typedef List<IExp> ExpList;
typedef List<IStm> StmList;

}
