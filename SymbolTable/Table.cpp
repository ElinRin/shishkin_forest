#include "Table.h"

#include "StringSymbol.h"
#include "Position.h"
#include "Symbol.h"
#include "DeclarationException.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>

namespace SymbolTable {

void Table::addClassToScope(const ClassInfo* classToScope)
{
    blocks.push_back(std::make_unique<ScopeBlock>(&classToScope->GetVariableBlock(),
                                &classToScope->GetMethodsBlock(), classToScope));
}

void Table::addMethodToScope(const MethodInfo* methodToScope)
{
    blocks.push_back(std::make_unique<ScopeBlock>(&methodToScope->GetVariableBlocks(), nullptr,
                                                  blocks.rbegin()->get()->currentClassInfo));
}

void Table::verifyClass(const ClassInfo* classToScope, const Position& position)
{
    std::set<const StringSymbol*> classesInGraph;
    for(auto classToCheck = classToScope; classToCheck->GetSuperClassName() != nullptr;
        classToCheck = this->GetClass(classToCheck->GetSuperClassName()->GetString(), position)) {
        if(classesInGraph.find(classToCheck->GetName()) != classesInGraph.end()) {
            throw DeclarationException("Cyclic dependency of class " +
                                           classToCheck->GetName()->GetString(),
                                           classToCheck->GetPosition());
        }
        classesInGraph.insert(classToScope->GetName());
    }
    for(auto classToCheck = classesInGraph.begin(); classToCheck != classesInGraph.end();
        ++classToCheck) {
        verifiedClasses.insert(*classToCheck);
    }
}

void Table::FreeLastScope()
{
    assert(blocks.size() > 0);
    blocks.pop_back();
}

void Table::AddClass(const ClassInfo* symbol, const Position& position)
{
    auto alreadyFound = classesBlock.find(symbol->GetName());
    if(alreadyFound != classesBlock.end()) {
        throw DeclarationException("Class " + symbol->GetName()->GetString() + " already defined at " +
                                   alreadyFound->second->GetName()->GetString() + "," + alreadyFound->second->GetPosition().ToString(),
                                   symbol->GetPosition());
    }
    classesNames.push_back(symbol->GetName());
    classesBlock.insert(std::make_pair(symbol->GetName(), std::unique_ptr<const ClassInfo>(symbol)));
}

void Table::AddClassToScope(const std::string& classToScopeName, const Position& position)
{
    auto classToAdd = GetClass(classToScopeName, position);
    if(verifiedClasses.find(classToAdd->GetName()) == verifiedClasses.end()) {
        verifyClass(classToAdd, position);
    }
    std::vector<const ClassInfo*> classesStack;
    classesStack.push_back(classToAdd);
    while (classToAdd->GetSuperClassName() != nullptr) {
        classToAdd = GetClass(classToAdd->GetSuperClassName()->GetString(), position);
        classesStack.push_back(classToAdd);
    }
    for(auto scopeToAdd = classesStack.rbegin(); scopeToAdd != classesStack.rend(); ++scopeToAdd ) {
        addClassToScope(*scopeToAdd);
    }
}

void Table::AddMethodToScope(const std::string& methodToScopeName, const Position& position)
{
    addMethodToScope(GetMethod(methodToScopeName, position));
}

const MethodInfo* Table::GetMethod(const std::string& name, const Position& position) const
{
    const StringSymbol* methodName = StringSymbol::GetIntern(name);
    for(auto block = blocks.rbegin(); block != blocks.rend(); ++block) {
        auto method = block->get()->methodBlock->find(methodName);
        if(method != block->get()->methodBlock->end()) {
            return method->second.get();
        }
    }
    throw DeclarationException("Not declared method " + name + " requested", position);
}

const VariableInfo* Table::GetVariable(const std::string& name, const Position& position) const
{
    const StringSymbol* variableName = StringSymbol::GetIntern(name);
    for(auto block = blocks.rbegin(); block != blocks.rend(); ++block) {
        auto variable = block->get()->variableBlock->find(variableName);
        if(variable != block->get()->variableBlock->end()) {
            return variable->second.get();
        }
    }
    throw DeclarationException("Not declared variable " + name + " requested", position);
}

bool Table::DoesTypeHaveSuper(const ClassInfo* classInfo, const StringSymbol *super, const Position position) const
{
    for(auto info = classInfo;
        info->GetSuperClassName() != nullptr;
        info = GetClass(info->GetSuperClassName()->GetString(), position))
    {
        if(classInfo->GetSuperClassName() == super) {
            return true;
        }
    }
    return false;
}

void Table::AddFrame(const StringSymbol* methodName, const ActivationRecords::IFrame* frame)
{
    Position position;
    if(frames.find(methodName) != frames.end()) {
        throw DeclarationException("Method " + methodName->GetString() + " already have defined frame", position);
    }
    frames.insert(std::make_pair(methodName, std::unique_ptr<const ActivationRecords::IFrame>(frame)));
}

const ActivationRecords::IFrame* Table::GetFrame(const StringSymbol *methodName) const
{
    Position position;
    auto frame = frames.find(methodName);
    if(frame == frames.end()) {
        throw DeclarationException("Method " + methodName->GetString() + " does not have a declared frame", position);
    }
    return frame->second.get();
}

const ClassInfo* Table::GetClass(const std::string& name, const Position& position) const
{
    const StringSymbol* className = StringSymbol::GetIntern(name);
    auto classInfo = classesBlock.find(className);
    if(classInfo != classesBlock.end()) {
        return classInfo->second.get();
    }
    throw DeclarationException("Not declared class " + name + " requested", position);
}

}	
