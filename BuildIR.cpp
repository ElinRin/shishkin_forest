#include "SymbolTable/TableFiller.h"

#include <iostream>

#include "tokens.h"
#include "DeclarationException.h"
#include "Table.h"
#include "TypeChecker.h"
#include "IRBuilder.h"
#include "IRPrinter.h"

#include <unistd.h>

extern std::unique_ptr<AST::Program> program;

int main(void) {
  //sleep(6);
  yyparse();
  SymbolTable::TableFiller filler;
  if(program.get()) {
      try {
        filler.FillTable(program.get());
        std::unique_ptr<SymbolTable::Table> symbolTable(filler.DetachTable());
        TypeChecker::TypeChecker checker(symbolTable.get());
        if( !checker.CheckAST(program.get()) ) {
            return 1;
        }
        filler.Attach(symbolTable.release());
        filler.FillClassStruct();
        symbolTable.reset(filler.DetachTable());

        IRTranslate::IRBuilder builder(symbolTable.get());
        builder.Parse(program.get());
        auto& forest = builder.GetParseResults();
        IRTranslate::IRPrinter printer("IR.dot");
        printer.CreateGraph(forest);

      } catch(SymbolTable::DeclarationException e) {
        std::cout << NF_RED << "Declaration error: " << e.what() << NF_RESET << std::endl;
        return 1;
      }
  }
  return 0;
}
