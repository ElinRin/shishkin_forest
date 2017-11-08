#include "TestSymbolTable/TableVisitor.h"
#include "tokens.h"
#include "Test.h"

extern std::unique_ptr<AST::Program> program;

int main(void) {
  TestClasses();
  yyparse();
  SymbolTable::TableVisitor visitor;
  visitor.ParseProgram(program.get());
}
