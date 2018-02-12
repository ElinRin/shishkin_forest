#include "common.h"

#include "IRVisitor.h"

namespace IR {

IR_VISIT_NODE(Unaryop)
IR_VISIT_NODE(Binop)
IR_VISIT_NODE(Call)
IR_VISIT_NODE(Const)
IR_VISIT_NODE(Eseq)
IR_VISIT_NODE(Mem)
IR_VISIT_NODE(Name)
IR_VISIT_NODE(Temp)

IR_VISIT_NODE(Exp)
IR_VISIT_NODE(Jump)
IR_VISIT_NODE(JumpC)
IR_VISIT_NODE(LabelStm)
IR_VISIT_NODE(Move)
IR_VISIT_NODE(Seq)

template<>
IR_VISIT_NODE(ExpList)
template<>
IR_VISIT_NODE(StmList)

}

namespace IRTranslate {

IR_VISIT_NODE(StmWrapper)
IR_VISIT_NODE(ExpWrapper)

}
