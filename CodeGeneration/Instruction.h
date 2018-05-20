#pragma once
#include <IRNodeTypes.h>

namespace CodeGeneration {

class IInstruction {
public:
    virtual ~IInstruction() {}

    const std::string AsmCode() const { return asmCode; }

    const IR::ConstTempList& UsedVars() const { return src; }
    const IR::ConstTempList& DefinedVars() const { return dst; }
    const IR::LabelList& JumpTargets() const { return labelList; }

    void RemoveUsed(const IR::Temp* used) {
        for (int i = 0; i < src.size(); ++i) {
            if ((*src[i]) == (*used)) {
                src.erase(src.begin() + i);
                return;
            }
        }
        assert(false);
    }

    void AddUsed(const IR::Temp* used) {
        src.push_back(used);
    }

    void RemoveDefined(const IR::Temp* defined) {
        for (int i = 0; i < dst.size(); ++i) {
            if ((*dst[i]) == (*defined)) {
                dst.erase(dst.begin() + i);
                return;
            }
        }
        assert(false);
    }

    void AddDefined(const IR::Temp* defined) {
        dst.push_back(defined);
    }

    virtual std::string Format() const {
        std::string s = asmCode;
        std::string instructionString;
        unsigned int pos;
        while ((pos = s.find("%")) < s.size()) {
            instructionString += s.substr(0, pos);
            if(s[pos+1] == 'l') {
                instructionString += labelList.at(0)->GetName();
            } else if(s[pos+1] - '0' < dst.size()) {
                assert(dst.size() > 0);
                instructionString += "r" + dst[0]->Name() + std::to_string(dst[0]->Id);
            } else {
                unsigned int srcPos = s[pos+1] - '0' - dst.size();
                assert(src.size() > srcPos);
                instructionString += "r" + src[srcPos]->Name() + std::to_string(src[srcPos]->Id);
            }
            s.erase(0, pos + 2);
        }
        instructionString += s;
        return instructionString;
    }

    virtual std::string FormatLong() const {
        std::string instructionString = Format();
        instructionString += "\tUsed:";
        for(auto& tmp: src) {
            instructionString += " r" + tmp->Name() + std::to_string(tmp->Id) + ";";
        }
        instructionString += "\tDefined:";
        for(auto& tmp: dst) {
            instructionString += " r" + tmp->Name() + std::to_string(tmp->Id) + ";";
        }
        return instructionString;
    }

protected:
    IR::ConstTempList src;
    IR::ConstTempList dst;
    IR::LabelList labelList;
    std::string asmCode;
};

class MoveInstruction : public IInstruction {
public:
    MoveInstruction(const IR::Temp* from, const IR::Temp* to, bool pureMove=false):
        pureMove(pureMove),
        fromConst(0)
    {
        src.push_back(from);
        dst.push_back(to);
    }
    MoveInstruction(const IR::ConstTempList&& from, bool pureMove=false):
        pureMove(pureMove)
    {
        src = from;
    }
    MoveInstruction(const IR::Const* from, const IR::Temp* to, bool pureMove=false):
        pureMove(pureMove),
        fromConst(from)
    { dst.push_back(to); }

    bool PureMove() const { return pureMove; }

private:
    const bool pureMove;
    const IR::Const* fromConst;
};

class LabelInstruction: public IInstruction {
public:
    LabelInstruction( const IR::Label* l ): label(l) {}

    const IR::Label* GetLabel() const { return label; }

    virtual std::string Format() const override {
        return label->GetName() + ":";
    }

private:
    const IR::Label* label;
};

struct InstructionList {
    std::vector<std::unique_ptr<IInstruction>> Instructions;
    // Registers defined during munching. DOES NOT INCLUDE ALL REGISTERS
    std::vector<std::unique_ptr<const IR::Temp>> Registers;
};

}
