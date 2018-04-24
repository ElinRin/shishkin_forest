#pragma once

#include <Instruction.h>

namespace CG = CodeGeneration;

namespace X86 {

enum Regs {
    EAX,
    EBX,
    ECX,
    EDX
};

class CISCOperation: public CG::IInstruction {
public:
    CISCOperation(std::string code, const IR::TempList&& srcList,
                  const IR::TempList&& dstList, const IR::LabelList&& targetLabels)
    {
        src = srcList;
        dst = dstList;
        labelList = targetLabels;
        asmCode = code;
    }

    CISCOperation(std::string code, const IR::TempList&& srcList,
                  const IR::TempList&& dstList)
    {
        src = srcList;
        dst = dstList;
        asmCode = code;
    }
};

class RegMove: public CG::MoveInstruction {
public:
    RegMove(const std::string code, const IR::Temp* from, const IR::Temp* to);
    RegMove(const std::string code, const IR::TempList&& from);
    RegMove(const std::string code, const IR::Const* from, const IR::Temp* to);
};

}
