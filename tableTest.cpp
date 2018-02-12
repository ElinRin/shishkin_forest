#include "TestSymbolTable/TableVisitor.h"

#include <iostream>

#include "tokens.h"
#include "Test.h"
#include "DeclarationException.h"

extern std::unique_ptr<AST::Program> program;

int main(void) {

  TestClasses();
  yyparse();
  SymbolTable::TableVisitor AST_VISITOR;
  if(program.get()) {
      try {
        AST_VISITOR.ParseProgram(program.get());
      } catch(SymbolTable::DeclarationException e) {
        std::cout << NF_RED << "Declaration error: " << e.what() << NF_RESET << std::endl;
      }
  }
  return 0;
}
