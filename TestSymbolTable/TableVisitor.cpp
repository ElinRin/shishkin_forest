#include "TableVisitor.h"

#include <iostream>

namespace SymbolTable {

void TableVisitor::ParseProgram(AST::Program* program)
{
    program->AcceptVisitor(this);
    for(auto className = table.GetClassesNames().begin();
        className != table.GetClassesNames().end(); ++className)
    {
        std::string name = (*className)->GetString();
        std::cout << "class " << name << ":" << std::endl;
        auto classInfo = table.GetClass(name, Position(0,0));
        table.AddClassToScope(name, Position(0,0));
        printClassInfo(classInfo);
        table.FreeLastScope();
    }
}

void TableVisitor::Visit(const AST::Program* node)
{
    node->Classes->AcceptVisitor(this);
}

void TableVisitor::Visit(const AST::MainClass* node)
{
    node->MainStatement->AcceptVisitor(this);
}

void TableVisitor::Visit(const AST::ClassDeclaration* node)
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
    table.AddClass(newClass, fromCoords(node->Coords()));
}

void TableVisitor::Visit(const AST::Sequence<const AST::ClassDeclaration>* node)
{
    auto& list = node->SequenceList;
    for(auto classDecl = list.begin(); classDecl != list.end(); ++classDecl) {
        classDecl->get()->AcceptVisitor(this);
    }
}

TypeInfo TableVisitor::fromType(const AST::Type* type)
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
        break;
    }
}

void TableVisitor::printClassInfo(const ClassInfo* classInfo)
{
    if(classInfo->GetSuperClassName() != nullptr) {
        std::cout << "\tSuper: " << classInfo->GetSuperClassName()->GetString() << std::endl;
        auto superClass = table.GetClass(classInfo->GetSuperClassName()->GetString(), Position(0, 0));
        printClassInfo(superClass);
    }
    std::cout << std::endl;

    for(auto varName = classInfo->GetVarsNames().begin();
        varName != classInfo->GetVarsNames().end(); ++varName)
    {
        std::cout << "\t\t";
        auto variable = table.GetVariable((*varName)->GetString(), Position(0, 0));
        printVariable(variable);
    }

    for(auto methodName = classInfo->GetMethodNames().begin();
        methodName != classInfo->GetMethodNames().end(); ++methodName) {
        auto method = table.GetMethod((*methodName)->GetString(), Position(0, 0));
        table.AddMethodToScope(method->GetName()->GetString(), Position(0, 0));
        std::cout << "\t\t" << (method->GetQualifier() == Q_Public ? "public " : "private ");
        std::cout << method->GetName()->GetString() << std::endl;
        std::cout << "\t\t\tArgs:" << std::endl;
        auto& args= method->GetArgsName();
        for(auto arg = args.begin(); arg != args.end(); ++arg) {
            auto var = table.GetVariable((*arg)->GetString(), Position(0, 0));
            std::cout << "\t\t\t\t";
            printVariable(var);
        }
        std::cout << "\t\t\tLocal vars:" << std::endl;
        auto& vars = method->GetVarsName();
        for(auto var = vars.begin(); var != vars.end(); ++var) {
            auto varInfo = table.GetVariable((*var)->GetString(), Position(0, 0));
            std::cout << "\t\t\t\t";
            printVariable(varInfo);
        }
        table.FreeLastScope();
    }
}

void TableVisitor::printVariable(const VariableInfo* varInfo)
{
    switch (varInfo->GetType().GetType()) {
    case VT_Int:
        std::cout << "int";
        break;
    case VT_IntArray:
        std::cout << "int[]";
        break;
    case VT_Boolean:
        std::cout << "boolean";
        break;
    case VT_UserClass:
        std::cout << "class " << varInfo->GetType().GetUserClass()->GetString();
        break;
    default:
        break;
    }
    std::cout << " " << varInfo->GetName()->GetString() << std::endl;
}

}
