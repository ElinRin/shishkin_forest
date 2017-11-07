#include "Table.h"
#include "ClassInfo.h"
#include "Position.h"

#include <iostream>
#include <assert.h>

/*
 * Первый тест
 * class A {
 *  int x;
 *  boolean b;
 *  int[] a;
 *
 *  B foo(boolean f) {
 *      int g;
 *  }
 * }
 *
 * class B : A {
 *  int y;
 *  bool foo1() {
 *      int[] b;
 *  }
 * }
 *
 * */

void TestClasses() {
    SymbolTable::Table table;
    auto aClass = new SymbolTable::ClassInfo("A", SymbolTable::Position(0, 0));
    auto xVariable = new SymbolTable::VariableInfo("x", SymbolTable::Position(1, 0),
                                                   SymbolTable::TypeInfo(SymbolTable::VT_Int));
    auto bVariable = new SymbolTable::VariableInfo("b", SymbolTable::Position(2, 0),
                                                   SymbolTable::TypeInfo(SymbolTable::VT_Boolean));
    auto aVariable = new SymbolTable::VariableInfo("a", SymbolTable::Position(3, 0),
                                                   SymbolTable::TypeInfo(SymbolTable::VT_IntArray));
    auto fooMethod = new SymbolTable::MethodInfo("foo", SymbolTable::Position(4, 0),
                                                   SymbolTable::TypeInfo(SymbolTable::VT_UserClass, "B"));
    auto fArg = new SymbolTable::VariableInfo("f", SymbolTable::Position(5, 0),
                                               SymbolTable::TypeInfo(SymbolTable::VT_Boolean));
    auto gLocal = new SymbolTable::VariableInfo("g", SymbolTable::Position(6, 0),
                                               SymbolTable::TypeInfo(SymbolTable::VT_IntArray));
    fooMethod->AddArgInfo(fArg);
    fooMethod->AddVariableInfo(gLocal);
    aClass->AddVariableInfo(xVariable);
    aClass->AddVariableInfo(bVariable);
    aClass->AddVariableInfo(aVariable);
    aClass->AddMethodInfo(fooMethod);
    table.AddClass(aClass, SymbolTable::Position(0, 0));

    auto bClass = new SymbolTable::ClassInfo("B", SymbolTable::Position(7, 0));
    bClass->AddSuperClassName("A");
    auto yVariable = new SymbolTable::VariableInfo("y", SymbolTable::Position(8, 0),
                                                   SymbolTable::TypeInfo(SymbolTable::VT_Int));
    auto bLocal = new SymbolTable::VariableInfo("b", SymbolTable::Position(9, 0),
                                                   SymbolTable::TypeInfo(SymbolTable::VT_IntArray));
    auto foo1Method = new SymbolTable::MethodInfo("foo1", SymbolTable::Position(10, 0),
                                                   SymbolTable::TypeInfo(SymbolTable::VT_Boolean));
    foo1Method->AddVariableInfo(bLocal);
    bClass->AddVariableInfo(yVariable);
    bClass->AddMethodInfo(foo1Method);
    table.AddClass(bClass, SymbolTable::Position(7, 0));

    assert(table.GetClass("B", SymbolTable::Position(11, 0))->GetMethodsBlock().size() == 1);
    table.AddClassToScope("B", SymbolTable::Position(12, 0));
    auto method = table.GetMethod("foo", SymbolTable::Position(12, 0));
    assert(method->GetVariableBlocks().find(method->GetArgsName().at(0))->second->GetType().GetType() ==
           SymbolTable::VT_Boolean);
    assert(table.GetVariable("b", SymbolTable::Position(12, 0))->GetType().GetType() == SymbolTable::VT_Boolean);
    table.AddMethodToScope("foo1", SymbolTable::Position(13, 0));
    assert(table.GetVariable("b", SymbolTable::Position(12, 0))->GetType().GetType() == SymbolTable::VT_IntArray);
}

int main() {
    TestClasses();
    return 0;
}
