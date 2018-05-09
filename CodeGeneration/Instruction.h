#pragma once
#include <IRNodeTypes.h>

namespace CodeGeneration {

class IInstruction {
public:
    virtual ~IInstruction() {}

    const std::string AsmCode() const { return asmCode; }

    const IR::TempList& UsedVars() const { return src; }
    const IR::TempList& DefinedVars() const { return dst; }
    const IR::LabelList& JumpTargets() const { return labelList; }

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
    IR::TempList src;
    IR::TempList dst;
    IR::LabelList labelList;
    std::string asmCode;
};

class MoveInstruction : public IInstruction {
public:
    MoveInstruction(const IR::Temp* from, const IR::Temp* to): fromConst(0)
    {
        src.push_back(from);
        dst.push_back(to);
    }
    MoveInstruction(const IR::TempList&& from)
    {
        src = from;
    }
    MoveInstruction(const IR::Const* from, const IR::Temp* to): fromConst(from) { dst.push_back(to); }

private:
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
    std::vector<std::unique_ptr<const IInstruction>> Instructions;
    std::vector<std::unique_ptr<const IR::Temp>> Registers;
};

}
