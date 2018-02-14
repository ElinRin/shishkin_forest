#pragma once
#include "common.h"

#include "IRNodeTypes.h"

#define IR_VISITOR(TREE_INTERFACE) virtual void Visit(const TREE_INTERFACE* node) = 0;
#define IR_VISIT_NODE(NODE_TYPE) void NODE_TYPE::AcceptVisitor(IR::IIRVisitor* visitor) const {\
    visitor->Visit(this); \
}

namespace IR {

interface IIRVisitor {

    IR_VISITOR(Unaryop)
    IR_VISITOR(Binop)
    IR_VISITOR(Call)
    IR_VISITOR(Const)
    IR_VISITOR(Eseq)
    IR_VISITOR(Mem)
    IR_VISITOR(Name)
    IR_VISITOR(Temp)

    IR_VISITOR(Exp)
    IR_VISITOR(Jump)
    IR_VISITOR(JumpC)
    IR_VISITOR(LabelStm)
    IR_VISITOR(Move)
    IR_VISITOR(Seq)

    IR_VISITOR(ExpList)

    IR_VISITOR(IRTranslate::StmWrapper)
    IR_VISITOR(IRTranslate::ExpWrapper)
};

}
