#include "SymbolTable/TableFiller.h"

#include <iostream>

#include "tokens.h"
#include "DeclarationException.h"
#include "Table.h"
#include "TypeChecker.h"
#include "FrameFiller.h"

extern std::unique_ptr<AST::Program> program;

int main(void) {
  yyparse();
  SymbolTable::TableFiller filler;
  if(program.get()) {
      try {
        filler.FillTable(program.get());
        std::unique_ptr<SymbolTable::Table> symbolTable(filler.DetachTable());
        TypeChecker::TypeChecker checker(symbolTable.get());
        checker.CheckAST(program.get());
        if( !checker.CheckAST(program.get()) ) {
            return 1;
        }
        filler.Attach(symbolTable.release());
        filler.FillClassStruct();
        symbolTable.reset(filler.DetachTable());
        ActivationRecords::FrameFiller filler(symbolTable.get());
        filler.PrintFill();
      } catch(SymbolTable::DeclarationException e) {
        std::cout << NF_RED << "Declaration error: " << e.what() << NF_RESET << std::endl;
        return 1;
      }
  }
  return 0;
}
