#ifndef TREETYPES_H
#define TREETYPES_H

// Тут дикая кольцевая зависимость, поэтому стоит включать только этот файл

#include "Visitor.h"

template <typename T>
VISIT_NODE(Sequence<T>)

#endif // TREETYPES_H
