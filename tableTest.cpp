#include "TestSymbolTable/TableVisitor.h"
#include "tokens.h"
#include "Test.h"

#include "unistd.h"

extern std::unique_ptr<AST::Program> program;

int main(void) {
  //sleep(10);
  TestClasses();
  yyparse();
  SymbolTable::TableVisitor visitor;
  visitor.ParseProgram(program.get());
}
