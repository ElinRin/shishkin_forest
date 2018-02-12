#include "ArrayStruct.h"
#include "IRNodeTypes.h"

namespace IR {

IExp* ArrayStruct::GetElement(const IExp* base, const IExp* elementNumber)
{
    return new Mem(
                new Binop(Binop::TB_PLUS,
                    new Binop(Binop::TB_PLUS, base, new Const(ElementsOffset)),
                    elementNumber));

}

IExp* ArrayStruct::GetLength(const IExp* base)
{
    return new Mem(base);
}

}
