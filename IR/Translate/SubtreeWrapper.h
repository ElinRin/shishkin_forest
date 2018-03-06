#pragma once
#include "common.h"

#include <unordered_map>
#include <vector>
#include "IExp.h"
#include "IStm.h"
#include "JumpC.h"
#include "StringSymbol.h"
#include "Label.h"

namespace IRTranslate {

interface ISubtreeWrapper {
    virtual ~ISubtreeWrapper() { }

    virtual IR::IExp* ToExp() = 0;
    virtual IR::IStm* ToStm() = 0;
    virtual IR::IStm* ToConditional(IR::JumpC::TJumpType, const IR::Label* trueLabel) = 0;
    virtual void AcceptVisitor(IR::IIRVisitor* visitor) = 0;
};

typedef std::vector<std::unique_ptr<IR::IStm>> LinearTree;
typedef std::unordered_map<const StringSymbol*, std::unique_ptr<ISubtreeWrapper>> IRForest;
typedef std::unordered_map<const StringSymbol*, LinearTree> IRLinearForest;
  
}
