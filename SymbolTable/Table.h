#pragma once

#include "Position.h"
#include "ClassInfo.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

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
    void AddClass(const ClassInfo *symbol, const Position &position); // может кинуть ошибку повторного объявления символа
    void AddClassToScope(const std::string& classToScopeName, const Position &position);
    void AddMethodToScope(const std::string&  methodToScopeName, const Position &position);
    const ClassInfo* GetClass(const std::string &name, const Position &position) const; // может кинуть ошибку необявленной переменной
    const MethodInfo* GetMethod(const std::string &name, const Position &position) const;
    const VariableInfo* GetVariable(const std::string &name, const Position &position) const;
    const std::vector< const StringSymbol* >& GetClassesNames() const  { return classesNames; }
    const ClassInfo* GetScopedClass() const { return blocks.size() > 0 ? blocks.rbegin()->get()->currentClassInfo : nullptr; }
    bool DoesTypeHaveSuper(const ClassInfo *classInfo, const StringSymbol *super, const Position position) const;

private:
    ClassBlock classesBlock;
    std::set<const StringSymbol*> verifiedClasses;
    std::vector< std::unique_ptr<ScopeBlock> > blocks;
    std::vector< const StringSymbol* > classesNames;

    void addClassToScope(const ClassInfo* classToScope);
    void addMethodToScope(const MethodInfo* methodToScope);
    void verifyClass(const ClassInfo* classToScope, const Position &position);
};

}
