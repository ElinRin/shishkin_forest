#include "SyntaxTree/TreeTypes.h"
#include "tokens.h"
#include "PrettyPrint/PrintVisitor.h"

extern std::unique_ptr<AST::Program> program;

int main(void) {

  yyparse();
  AST::PrintVisitor printer("tree.dot");
  printer.CreateGraph(program.get());
}
