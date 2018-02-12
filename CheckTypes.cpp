#include "SymbolTable/TableFiller.h"

#include <iostream>

#include "tokens.h"
#include "DeclarationException.h"
#include "Table.h"
#include "TypeChecker.h"
#include <unistd.h>

extern std::unique_ptr<AST::Program> program;

int main(void) {
  //sleep(10);
  yyparse();
  if(program.get()) {
      try {
        SymbolTable::TableFiller filler;
        filler.FillTable(program.get());
        std::unique_ptr<SymbolTable::Table> symbolTable(filler.DetachTable());
        TypeChecker::TypeChecker checker(symbolTable.get());
        if( !checker.CheckAST(program.get()) ) {
            return 1;
        }
        filler.Attach(symbolTable.release());
        filler.FillClassStruct();
        symbolTable.reset(filler.DetachTable());
      } catch(SymbolTable::DeclarationException e) {
        std::cout << NF_RED << "Declaration error: " << e.what() << NF_RESET << std::endl;
        return 1;
      }
  }
  std::cout << "No issues detected" << std::endl;
  return 0;
}
