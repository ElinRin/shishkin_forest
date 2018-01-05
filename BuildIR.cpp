#include "SymbolTable/TableFiller.h"

#include <iostream>
#include <unistd.h>

#include "tokens.h"
#include "DeclarationException.h"
#include "Table.h"
#include "TypeChecker.h"

extern std::unique_ptr<AST::Program> program;

int main(void) {
    //sleep(10);
  yyparse();
  SymbolTable::TableFiller filler;
  if(program.get()) {
      try {
        filler.FillTable(program.get());
        std::unique_ptr<SymbolTable::Table> symbolTable(filler.DetachTable());
        TypeChecker::TypeChecker checker;
        checker.CheckAST(program.get(), symbolTable.get());
      } catch(SymbolTable::DeclarationException e) {
        std::cout << NF_RED << "Declaration error: " << e.what() << NF_RESET << std::endl;
      }
  }
  return 0;
}
