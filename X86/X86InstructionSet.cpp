#include <X86InstructionSet.h>

namespace X86 {

RegMove::RegMove(const std::string code, const IR::Temp* from, const IR::Temp* to, bool pureMove):
    MoveInstruction(from, to, pureMove)
{
    asmCode = code;
}

RegMove::RegMove(const std::string code, const IR::ConstTempList&& from, bool pureMove):
    MoveInstruction(std::move(from), pureMove)
{
    asmCode = code;
}

RegMove::RegMove(const std::string code, const IR::Const* from, const IR::Temp* to, bool pureMove):
    MoveInstruction(from, to, pureMove)
{
    asmCode = code;
}

}
