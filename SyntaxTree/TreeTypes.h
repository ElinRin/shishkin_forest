#ifndef TREETYPES_H
#define TREETYPES_H

// Тут дикая кольцевая зависимость, поэтому стоит включать только этот файл

#include "Visitor.h"

namespace AST {

template <typename T>
AST_VISIT_NODE(Sequence<T>)

}

#endif // TREETYPES_H
