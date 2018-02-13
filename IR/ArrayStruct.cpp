#include "ArrayStruct.h"
#include "IRNodeTypes.h"

namespace IR {

IExp* ArrayStruct::GetElement(const IExp* base, const IExp* elementNumber, const IR::Coords& coords)
{
    return new Mem(
                new Binop(Binop::TB_PLUS,
                    new Binop(Binop::TB_PLUS, base, new Const(ElementsOffset, coords), coords),
                    elementNumber, coords), coords);

}

IExp* ArrayStruct::GetLength(const IExp* base, const Coords &coords)
{
    return new Mem(base, coords);
}

}
