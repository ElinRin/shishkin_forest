#include "TypeChecker.h"

#include <iostream>
#include "StringSymbol.h"
#include "DeclarationException.h"
#include "assert.h"

namespace TypeChecker {

static SymbolTable::TypeInfo IntType(SymbolTable::VT_Int);
static SymbolTable::TypeInfo BooleanType(SymbolTable::VT_Boolean);
static SymbolTable::TypeInfo IntArrayType(SymbolTable::VT_IntArray);

inline static SymbolTable::Position fromCoords(const AST::Coordinates& coords)
{
    return SymbolTable::Position(coords.Column, coords.Row);
}

static SymbolTable::TypeInfo fromType(const AST::Type* type)
{
    switch (type->TypeEnum) {
    case AST::T_Int:
        return SymbolTable::TypeInfo(SymbolTable::VT_Int);
        break;
    case AST::T_IntArray:
        return SymbolTable::TypeInfo(SymbolTable::VT_IntArray);
        break;
    case AST::T_Boolean:
        return SymbolTable::TypeInfo(SymbolTable::VT_Boolean);
        break;
    case AST::T_UserType:
        return SymbolTable::TypeInfo(SymbolTable::VT_UserClass, type->TypeName->Name);
        break;
    default:
        assert(false);
        break;
    }
}

inline bool operator ==(const AST::Type &a, const AST::Type &b) {
    if(a.TypeEnum != AST::T_UserType) {
        return a.TypeEnum == b.TypeEnum;
    }
    auto aSymbol = StringSymbol::GetIntern(a.TypeName->Name);
    auto bSymbol = StringSymbol::GetIntern(b.TypeName->Name);
    return a.TypeEnum == b.TypeEnum &&
             aSymbol == bSymbol;
}

TypeChecker::TypeChecker(SymbolTable::Table* table):
    table(table),
    TypeStackVisitor(table)
{
}

bool TypeChecker::CheckAST(AST::Program* root)
{
    try {
        root->AcceptVisitor(this);
    } catch (SymbolTable::DeclarationException e) {
        std::cout << NF_RED << "Declaration error: " << e.what() << NF_RESET << std::endl;
        return false;
    }
    return true;
}

void TypeChecker::Visit(const AST::Program* node)
{
    node->Main->AcceptVisitor(this);
    node->Classes->AcceptVisitor(this);
}

void TypeChecker::Visit(const AST::MainClass *node)
{
    node->MainStatement->AcceptVisitor(this);
}

void TypeChecker::Visit(const AST::ClassDeclaration* node)
{
    SymbolTable::TypeScopeSwitcher switcher(node->ClassName->Name, table,
                                            fromCoords(node->Coords()));
    if(node->MethodDeclarations) {
        node->MethodDeclarations->AcceptVisitor(this);
    }
}

void TypeChecker::Visit(const AST::VarDeclaration* node)
{
    if(node->VarType->TypeEnum == AST::T_UserType) {
        table->GetClass(node->VarType->TypeName->Name, fromCoords(node->Coords()));
    }
}

void TypeChecker::Visit(const AST::MethodDeclaration* node)
{
    SymbolTable::MethodScopeSwitcher switcher(node->MethodName->Name, table,
                                              fromCoords(node->Coords()));
    if(node->Arguments) {
        node->Arguments->AcceptVisitor(this);
    }
    if(node->Statements) {
        node->Statements->AcceptVisitor(this);
    }
    if(node->ReturnType->TypeEnum == AST::T_UserType) {
        table->GetClass(node->ReturnType->TypeName->Name, fromCoords(node->Coords()));
    }
    node->StatementToReturn->AcceptVisitor(this);
    auto returnedType = TypeStackVisitor.PopTypeFromStack();
    if( !(*returnedType == fromType(node->ReturnType.get())) ) {
        throw SymbolTable::DeclarationException("Trying to return type " +
                                                returnedType->GetTypeString() +
                                                " from method " +
                                                node->MethodName->Name +
                                                " of type " + node->ReturnType->ToString(),
                                                fromCoords(node->Coords()));
    }
}

template <class T>
static void visitSequence(const AST::Sequence<T>* node, AST::IVisitor* AST_VISITOR)
{
    for(auto& cl: node->SequenceList) {
        cl->AcceptVisitor(AST_VISITOR);
    }
}

void TypeChecker::Visit(const AST::Sequence<const AST::ClassDeclaration>* node)
{
    visitSequence(node, this);
}

void TypeChecker::Visit(const AST::Sequence<const AST::VarDeclaration>* node)
{
    visitSequence(node, this);
}

void TypeChecker::Visit(const AST::Sequence<const AST::MethodDeclaration>* node)
{
    visitSequence(node, this);
}

void TypeChecker::Visit(const AST::Qualifier* node)
{
    assert(false);
}

void TypeChecker::Visit(const AST::Type* node)
{
    if(node->TypeEnum == AST::T_UserType) {
        table->GetClass(node->TypeName->Name, fromCoords(node->Coords()));
    }
}

void TypeChecker::Visit(const AST::ReturnStatement* node)
{
    node->Expression->AcceptVisitor(this);
}

void TypeChecker::Visit(const AST::AssignArrayElementStatement* node)
{
    auto array = table->GetVariable(node->Identifier->Name, fromCoords(node->Coords()));
    if(array->GetType().GetType() != SymbolTable::VT_IntArray) {
        throw SymbolTable::DeclarationException("Trying to subscript" +
                                                        array->GetType().GetTypeString() +
                                                        " as array",
                                                        fromCoords(node->Coords()));
    }
    node->ElementNumberExpression->AcceptVisitor(this);
    auto returned = TypeStackVisitor.PopTypeFromStack();
    if(returned->GetType() != SymbolTable::VT_Int) {
        throw SymbolTable::DeclarationException("Trying to return type " +
                                                        returned->GetTypeString() +
                                                        " as index of array " +
                                                        node->Identifier->Name,
                                                        fromCoords(node->Coords()));
    }
    node->ExpressionToAssign->AcceptVisitor(this);
    returned = TypeStackVisitor.PopTypeFromStack();
    if(returned->GetType() != SymbolTable::VT_Int) {
            throw SymbolTable::DeclarationException("Trying to assign type " +
                                                            returned->GetTypeString() +
                                                            " to int array " +
                                                            node->Identifier->Name,
                                                            fromCoords(node->Coords()));
    }
}

void TypeChecker::Visit(const AST::AssignStatement* node)
{
    auto var = table->GetVariable(node->Identifier->Name, fromCoords(node->Coords()));
    node->ExpressionToAssign->AcceptVisitor(this);
    auto returnedType = TypeStackVisitor.PopTypeFromStack();
    if( !(var->GetType() == *returnedType) ) {
        throw SymbolTable::DeclarationException("Trying to assign type " +
                                                        returnedType->GetTypeString() +
                                                        " to variable " +
                                                        node->Identifier->Name +
                                                        " of type " +
                                                        var->GetType().GetTypeString(),
                                                        fromCoords(node->Coords()));
    }
}

void TypeChecker::Visit(const AST::PrintLineStatement* node)
{
    node->ExpressionToPrint->AcceptVisitor(this);
    auto toPrint = TypeStackVisitor.PopTypeFromStack();
    if(toPrint->GetType() == SymbolTable::VT_UserClass) {
        throw SymbolTable::DeclarationException("Trying to return user type " +
                                                        toPrint->GetTypeString(),
                                                        fromCoords(node->Coords()));
    }
}

void TypeChecker::Visit(const AST::WhileStatement* node)
{
    node->Condition->AcceptVisitor(this);
    auto returned = TypeStackVisitor.PopTypeFromStack();
    if(returned->GetType() != SymbolTable::VT_Boolean) {
            throw SymbolTable::DeclarationException("Trying to use type " +
                                                            returned->GetTypeString() +
                                                            " as while condition",
                                                            fromCoords(node->Coords()));
    }
    node->Body->AcceptVisitor(this);
}

void TypeChecker::Visit(const AST::BraceSequenceStatement* node)
{
    node->BracedSequence->AcceptVisitor(this);
}

void TypeChecker::Visit(const AST::IfElseStatement* node)
{
    node->Condition->AcceptVisitor(this);
    auto returned = TypeStackVisitor.PopTypeFromStack();
    if(returned->GetType() != SymbolTable::VT_Boolean) {
            throw SymbolTable::DeclarationException("Trying to use type " +
                                                            returned->GetTypeString() +
                                                            " as if condition",
                                                            fromCoords(node->Coords()));
    }
    node->IfStatement->AcceptVisitor(this);
    node->ElseStatement->AcceptVisitor(this);
}

void TypeChecker::Visit(const AST::Sequence<const AST::IStatement>* node)
{
    visitSequence(node, this);
}

void TypeChecker::Visit(const AST::BinaryExpression* node)
{
    node->Left->AcceptVisitor(this);
    auto returned = TypeStackVisitor.PopTypeFromStack();
    switch (node->Type) {
    case AST::BET_Plus:
    case AST::BET_Minus:
    case AST::BET_Mult:
    case AST::BET_Mod:
    case AST::BET_Less:
        if(returned->GetType() != SymbolTable::VT_Int) {
                throw SymbolTable::DeclarationException("Trying to apply math operation to " +
                                                                returned->GetTypeString(),
                                                                fromCoords(node->Coords()));
        }
        node->Right->AcceptVisitor(this);
        returned = TypeStackVisitor.PopTypeFromStack();
        if(returned->GetType() != SymbolTable::VT_Int) {
                throw SymbolTable::DeclarationException("Trying to apply math operation to " +
                                                                returned->GetTypeString(),
                                                                fromCoords(node->Coords()));
        }
        break;
    case AST::BET_And:
    case AST::BET_Or:
        if(returned->GetType() != SymbolTable::VT_Boolean) {
                throw SymbolTable::DeclarationException("Trying to apply logical operation to " +
                                                                returned->GetTypeString(),
                                                                fromCoords(node->Coords()));
        }
        node->Right->AcceptVisitor(this);
        returned = TypeStackVisitor.PopTypeFromStack();
        if(returned->GetType() != SymbolTable::VT_Boolean) {
                throw SymbolTable::DeclarationException("Trying to apply logical operation to " +
                                                                returned->GetTypeString(),
                                                                fromCoords(node->Coords()));
        }
    default:
        break;
    }
    TypeStackVisitor.Visit(node);

}

void TypeChecker::Visit(const AST::ArrayMemberExpression* node)
{
    node->BaseExpression->AcceptVisitor(this);
    auto returnedBase = TypeStackVisitor.PopTypeFromStack();
    if(returnedBase->GetType() != SymbolTable::VT_IntArray) {
        throw SymbolTable::DeclarationException("Trying to use type " +
                                                        returnedBase->GetTypeString() +
                                                        " as array",
                                                        fromCoords(node->Coords()));
    }
    node->ElementNumberExpression->AcceptVisitor(this);
    auto returned = TypeStackVisitor.PopTypeFromStack();
    if(returned->GetType()!= SymbolTable::VT_Int) {
        throw SymbolTable::DeclarationException("Trying to return type " +
                                                        returned->GetTypeString() +
                                                        " as index of array ",
                                                        fromCoords(node->Coords()));
    }
    TypeStackVisitor.Visit(node);
}

void TypeChecker::TypeChecker::Visit(const AST::ArrayLengthExpression* node)
{
    node->ArrayExpression->AcceptVisitor(this);
    auto returnedBase = TypeStackVisitor.PopTypeFromStack();
    if(returnedBase->GetType() != SymbolTable::VT_IntArray) {
        throw SymbolTable::DeclarationException("Trying to use type " +
                                                        returnedBase->GetTypeString() +
                                                        " as array",
                                                        fromCoords(node->Coords()));
    }
    TypeStackVisitor.Visit(node);
}

void TypeChecker::TypeChecker::Visit(const AST::CallMemberExpression* node)
{
    node->BaseExpression->AcceptVisitor(this);
    auto returnedBase = TypeStackVisitor.GetTypeFromStack();
    if(returnedBase->GetType() != SymbolTable::VT_UserClass) {
        throw SymbolTable::DeclarationException("Trying to use type " +
                                                        returnedBase->GetTypeString() +
                                                        " as user type",
                                                        fromCoords(node->Coords()));
    }
    auto classInfo = table->GetClass(returnedBase->GetTypeString(), fromCoords(node->Coords()));
    auto methodIterator = classInfo->GetMethodsBlock().find(StringSymbol::GetIntern(node->CalledMember->Name));
    if(methodIterator == classInfo->GetMethodsBlock().end()) {
        throw SymbolTable::DeclarationException("Requested method " +
                                                        node->CalledMember->Name +
                                                        " is not present in class " +
                                                        classInfo->GetName()->GetString(),
                                                        fromCoords(node->Coords()));
    }
    const SymbolTable::MethodInfo* methodInfo = methodIterator->second.get();
    auto scoped = table->GetScopedClass();
    if(methodInfo->GetQualifier() == SymbolTable::Q_Private && (scoped == nullptr || scoped->GetName() != classInfo->GetName())) {
        throw SymbolTable::DeclarationException("Requested method " + classInfo->GetName()->GetString() +
                                                        "::" + node->CalledMember->Name +
                                                        " has private qualifier",
                                                        fromCoords(node->Coords()));
    }
    if(node->ArgumentSequence) {
        node->ArgumentSequence->AcceptVisitor(this);
        if(node->ArgumentSequence->SequenceList.size() != static_cast<unsigned int>(methodInfo->GetArgsCount())) {
            throw SymbolTable::DeclarationException("Requested method " + classInfo->GetName()->GetString() +
                                                            "::" + node->CalledMember->Name +
                                                            " has " + std::to_string(methodInfo->GetArgsCount()) +
                                                            " arguments, but " + std::to_string(node->ArgumentSequence->SequenceList.size()) +
                                                            " passed",
                                                            fromCoords(node->Coords()));
        }
        for(auto argument = methodInfo->GetArgs().rbegin(); argument != methodInfo->GetArgs().rend(); ++argument) {
            auto arg = *argument;
            auto passed = TypeStackVisitor.PopTypeFromStack();
            if( !(arg->GetType() == *passed) ) {
                if(passed->GetType() == SymbolTable::VT_UserClass &&
                        table->DoesTypeHaveSuper(table->GetClass(passed->GetUserClass()->GetString(), fromCoords(node->Coords())),
                                                 arg->GetType().GetUserClass(), fromCoords(node->Coords()))) {
                    continue;
                }
                throw SymbolTable::DeclarationException("Requested method " + classInfo->GetName()->GetString() +
                                                                "::" + node->CalledMember->Name +
                                                                " was called with invalid argument - expected " +
                                                                arg->GetType().GetTypeString() +
                                                                ", but got " + passed->GetTypeString(),
                                                                fromCoords(node->Coords()));
            }
        }
    }
    TypeStackVisitor.Visit(node);
}

void TypeChecker::Visit(const AST::Sequence<const AST::IExpression>* node)
{
    visitSequence(node, this);
}

void TypeChecker::Visit(const AST::ValueExpression* node)
{
    TypeStackVisitor.Visit(node);
}

void TypeChecker::Visit(const AST::IdExpression* node)
{
    TypeStackVisitor.Visit(node);
}

void TypeChecker::Visit(const AST::ThisExpression* node)
{
    TypeStackVisitor.Visit(node);
}

void TypeChecker::Visit(const AST::NewIntArrayExpression* node)
{
    node->NumberOfElements->AcceptVisitor(this);
    auto returned = TypeStackVisitor.PopTypeFromStack();
    if(returned->GetType()!= SymbolTable::VT_Int) {
        throw SymbolTable::DeclarationException("Trying to use type " +
                                                        returned->GetTypeString() +
                                                        " as size of array ",
                                                        fromCoords(node->Coords()));
    }
    TypeStackVisitor.Visit(node);
}

void TypeChecker::Visit(const AST::NewObjectExpression* node)
{
    TypeStackVisitor.Visit(node);
}

void TypeChecker::TypeChecker::Visit(const AST::NotExpression* node)
{
    node->Expression->AcceptVisitor(this);
    auto returned = TypeStackVisitor.PopTypeFromStack();
    if(returned->GetType() != SymbolTable::VT_Boolean) {
        throw SymbolTable::DeclarationException("Trying to use type " +
                                                        returned->GetTypeString() +
                                                        " as boolean expression ",
                                                        fromCoords(node->Coords()));
    }
    TypeStackVisitor.Visit(node);
}

void TypeChecker::TypeChecker::Visit(const AST::ContainerExpression *node)
{
    node->Expression->AcceptVisitor(this);
}

void TypeChecker::TypeChecker::Visit(const AST::Id *node)
{
    assert(false);
}

}

