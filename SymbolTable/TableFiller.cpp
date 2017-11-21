#include "TableFiller.h"

#include <assert.h>
#include <iostream>
#include "DeclarationException.h"

void SymbolTable::TableFiller::FillTable(AST::Program *program)
{
    program->AcceptVisitor(this);
}

void SymbolTable::TableFiller::Visit(const AST::Program* node)
{
    node->Classes->AcceptVisitor(this);
}

void SymbolTable::TableFiller::Visit(const AST::MainClass* node)
{
    node->MainStatement->AcceptVisitor(this);
}

void SymbolTable::TableFiller::Visit(const AST::ClassDeclaration* node)
{
    auto newClass = new ClassInfo(node->ClassName->Name, fromCoords(node->Coords()));
    if(node->SuperName.get() != nullptr) {
        newClass->AddSuperClassName(node->SuperName->Name);
    }
    auto varDeclarations = node->VarDeclarations.get();
    if(varDeclarations != nullptr) {
        auto& varSequence = varDeclarations->SequenceList;
        for(auto var = varSequence.begin(); var != varSequence.end(); ++var) {
            newClass->AddVariableInfo(new VariableInfo(var->get()->Identifier->Name,
                                                       fromCoords(var->get()->Coords()),
                                                       fromType(var->get()->VarType.get())));
        }
    }
    auto methodDeclarations = node->MethodDeclarations.get();
    if(methodDeclarations != nullptr) {
        auto& methodSequence = methodDeclarations->SequenceList;
        for(auto method = methodSequence.begin(); method != methodSequence.end(); ++method) {
            auto methodInfo = new MethodInfo(method->get()->MethodName->Name,
                                             node->ClassName->Name,
                                    fromCoords(method->get()->Coords()),
                                    fromType(method->get()->ReturnType.get()),
                                    method->get()->QualifierType->QualifierEnum == AST::Q_Public ? Q_Public : Q_Private);
            auto argDeclarations = method->get()->Arguments.get();
            if(argDeclarations != nullptr) {
                auto& args = argDeclarations->SequenceList;
                for(auto arg = args.begin(); arg != args.end(); ++arg) {
                    methodInfo->AddArgInfo(new VariableInfo(arg->get()->Identifier->Name,
                                                            fromCoords(arg->get()->Coords()),
                                                            fromType(arg->get()->VarType.get())));
                }
            }
            auto varDeclarations = method->get()->VarDeclarations.get();
            if(varDeclarations != nullptr) {
                auto& localVariables = varDeclarations->SequenceList;
                for(auto var = localVariables.begin(); var != localVariables.end(); ++var) {
                    methodInfo->AddVariableInfo(new VariableInfo(var->get()->Identifier->Name,
                                                               fromCoords(var->get()->Coords()),
                                                               fromType(var->get()->VarType.get())));
                }
            }
            newClass->AddMethodInfo(methodInfo);
        }
    }
    table->AddClass(newClass, fromCoords(node->Coords()));
}

void SymbolTable::TableFiller::Visit(const AST::Sequence<const AST::ClassDeclaration>* node)
{
    auto& list = node->SequenceList;
    for(auto classDecl = list.begin(); classDecl != list.end(); ++classDecl) {
        classDecl->get()->AcceptVisitor(this);
    }
}

SymbolTable::TypeInfo SymbolTable::TableFiller::fromType(const AST::Type* type)
{
    switch (type->TypeEnum) {
    case AST::T_Int:
        return TypeInfo(VT_Int);
        break;
    case AST::T_IntArray:
        return TypeInfo(VT_IntArray);
        break;
    case AST::T_Boolean:
        return TypeInfo(VT_Boolean);
        break;
    case AST::T_UserType:
        return TypeInfo(VT_UserClass, type->TypeName->Name);
        break;
    default:
        assert(false);
        break;
    }
}
