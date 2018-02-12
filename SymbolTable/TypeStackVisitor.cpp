#include "TypeStackVisitor.h"

#include <assert.h>

namespace SymbolTable {

static SymbolTable::TypeInfo IntType(SymbolTable::VT_Int);
static SymbolTable::TypeInfo BooleanType(SymbolTable::VT_Boolean);
static SymbolTable::TypeInfo IntArrayType(SymbolTable::VT_IntArray);

inline static SymbolTable::Position fromCoords(const AST::Coordinates& coords)
{
    return SymbolTable::Position(coords.Column, coords.Row);
}

TypeStackVisitor::TypeStackVisitor(Table* table):table(table)
{
}

const TypeInfo* TypeStackVisitor::PopTypeFromStack()
{
    if(typesStack.size() > 0) {
        return popTypeStack();
    }
    return nullptr;
}

const TypeInfo* TypeStackVisitor::GetTypeFromStack() const
{
    if(typesStack.size() > 0) {
        return typesStack.top();
    }
    return nullptr;
}

void TypeStackVisitor::Visit(const AST::BinaryExpression* node)
{
    switch (node->Type) {
    case AST::BET_Plus:
    case AST::BET_Minus:
    case AST::BET_Mult:
    case AST::BET_Mod:
        typesStack.push(&IntType);
        break;
    case AST::BET_Less:
    case AST::BET_And:
    case AST::BET_Or:
        typesStack.push(&BooleanType);
        break;
    default:
        assert(false);
        break;
    }
}

void TypeStackVisitor::Visit(const AST::ArrayMemberExpression* node)
{
    typesStack.push(&IntType);
}

void TypeStackVisitor::Visit(const AST::ArrayLengthExpression* node)
{
    typesStack.push(&IntType);
}

void TypeStackVisitor::Visit(const AST::CallMemberExpression* node)
{
    auto returnedBase = popTypeStack();
    Position pos = fromCoords(node->Coords());
    TypeScopeSwitcher switcher(*returnedBase, table, pos);
    auto methodInfo = table->GetMethod(node->CalledMember->Name, pos);
    typesStack.push(&methodInfo->GetReturnType());
}

void TypeStackVisitor::Visit(const AST::ValueExpression* node)
{
    switch(node->ValueType) {
    case AST::VT_Int:
        typesStack.push(&IntType);
        break;
    case AST::VT_Boolean:
        typesStack.push(&BooleanType);
        break;
    default:
        assert(false);
        break;
    }
}

void TypeStackVisitor::Visit(const AST::IdExpression* node)
{
    auto var = table->GetVariable(node->ExpressionId->Name, fromCoords(node->Coords()));
    typesStack.push(&var->GetType());
}

void TypeStackVisitor::Visit(const AST::ThisExpression* node)
{
    auto scopedClass = table->GetScopedClass();
    typesStack.push(&scopedClass->GetTypeInfo());
}

void TypeStackVisitor::Visit(const AST::NewIntArrayExpression* node)
{
    typesStack.push(&IntArrayType);
}

void TypeStackVisitor::Visit(const AST::NewObjectExpression* node)
{
    auto objectType = table->GetClass(node->ObjectId->Name, fromCoords(node->Coords()));
    typesStack.push(&objectType->GetTypeInfo());
}

void TypeStackVisitor::Visit(const AST::NotExpression* node)
{
    typesStack.push(&BooleanType);
}

TypeScopeSwitcher::TypeScopeSwitcher(const TypeInfo &type, Table* table,
                                     const Position& position):
    table(table),
    type(type)
{
    if(type.GetType() == VT_UserClass) {
        table->AddClassToScope(type.GetUserClass()->GetString(), position);
    }
}

TypeScopeSwitcher::TypeScopeSwitcher(const std::string& className,
                                     Table* table, const Position& position):
    table(table),
    type(table->GetClass(className, position)->GetTypeInfo())
{
    table->AddClassToScope(className, position);
}

TypeScopeSwitcher::~TypeScopeSwitcher()
{
    if(type.GetType() == VT_UserClass) {
        table->FreeLastScope();
    }
}

MethodScopeSwitcher::MethodScopeSwitcher(const MethodInfo* method, Table* table,
                                         const Position& position):
    table(table)
{
    table->AddMethodToScope(method->GetName()->GetString(), position);
}

MethodScopeSwitcher::MethodScopeSwitcher(const std::string &name, Table* table,
                                         const Position& position):
    table(table)
{
    table->AddMethodToScope(name, position);
}

MethodScopeSwitcher::~MethodScopeSwitcher()
{
    table->FreeLastScope();
}

}
