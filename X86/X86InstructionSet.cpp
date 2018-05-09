#include <X86InstructionSet.h>

namespace X86 {

RegMove::RegMove(const std::string code, const IR::Temp* from, const IR::Temp* to):
    MoveInstruction(from, to)
{
    asmCode = code;
}

RegMove::RegMove(const std::string code, const IR::ConstTempList&& from):
    MoveInstruction(std::move(from))
{
    asmCode = code;
}

RegMove::RegMove(const std::string code, const IR::Const* from, const IR::Temp* to):
    MoveInstruction(from, to)
{
    asmCode = code;
}

}
