#pragma once

#include "Position.h"
#include "ClassInfo.h"
#include "Frame.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

namespace AR = ActivationRecords;

namespace SymbolTable {

struct ScopeBlock
{
    ScopeBlock(const VariableBlock* _variableBlock = nullptr,
               const MethodBlock* _methodBlock = nullptr,
               const ClassInfo* _currentClassInfo = nullptr) :
        methodBlock(_methodBlock),
        variableBlock(_variableBlock),
        currentClassInfo(_currentClassInfo) {
    }
    const MethodBlock* methodBlock;
    const VariableBlock* variableBlock;
    const ClassInfo* currentClassInfo;
};

class Table {
public:
    void FreeLastScope();
    void AddClass(const ClassInfo *symbol, const Position &position = Position()); // может кинуть ошибку повторного объявления символа
    void AddClassToScope(const std::string& classToScopeName, const Position &position = Position());
    void AddMethodToScope(const std::string&  methodToScopeName, const Position &position = Position());
    const ClassInfo* GetClass(const std::string &name, const Position &position = Position()) const; // может кинуть ошибку необявленной переменной
    const MethodInfo* GetMethod(const std::string &name, const Position &position = Position()) const;
    const VariableInfo* GetVariable(const std::string &name, const Position &position = Position()) const;
    const std::vector< const StringSymbol* >& GetClassesNames() const  { return classesNames; }
    const ClassInfo* GetScopedClass() const { return blocks.size() > 0 ? blocks.rbegin()->get()->currentClassInfo : nullptr; }
    bool DoesTypeHaveSuper(const ClassInfo *classInfo, const StringSymbol *super, const Position position = Position()) const;
    void AddFrame(const StringSymbol* methodName, const AR::IFrame* frame);
    const ActivationRecords::IFrame* GetFrame(const StringSymbol* methodName) const;

private:
    ClassBlock classesBlock;
    std::set<const StringSymbol*> verifiedClasses;
    std::vector< std::unique_ptr<ScopeBlock> > blocks;
    std::vector< const StringSymbol* > classesNames;
    std::unordered_map< const StringSymbol*, const AR::IFrame*> frames;

    void addClassToScope(const ClassInfo* classToScope);
    void addMethodToScope(const MethodInfo* methodToScope);
    void verifyClass(const ClassInfo* classToScope, const Position &position);
};

}
