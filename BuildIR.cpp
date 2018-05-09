#include "SymbolTable/TableFiller.h"

#include <iostream>
#include <unistd.h>

#include "tokens.h"
#include "DeclarationException.h"
#include "Table.h"
#include "TypeChecker.h"
#include "IRBuilder.h"
#include "IRPrinter.h"
#include "EseqCanonizer.h"
#include "Linerizer.h"
#include "NoJumpBlock.h"
#include "X86CodeGeneration.h"
#include "LifecycleGraph.h"
#include "LifecyclePrinter.h"

extern std::unique_ptr<AST::Program> program;

int main(void) {
  yyparse();
  SymbolTable::TableFiller filler;
  if(program.get()) {
      try {
        //sleep(6);
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
        IRTranslate::EseqCanonizer canonizer;
        for(auto& tree: forest) {
            tree.second.reset(canonizer.Canonize(tree.second.get()));
        }
        IRTranslate::IRPrinter printerCan("can_IR.dot");
        printerCan.CreateGraph(forest);
        IRTranslate::Linerizer linerizer;
        IRTranslate::IRLinearForest linerized;
        for(auto& tree: forest) {
            linerized.insert(std::make_pair(tree.first, std::vector<std::unique_ptr<IR::IStm>>()));
            linerizer.Linerize(tree.second.get(), linerized.at(tree.first));
        }
        IRTranslate::IRPrinter printerLin("lin_IR.dot");
        printerLin.CreateGraph(linerized);

        IRTranslate::NoJumpBlocksForest noJumpForest;
        for(auto& tree: linerized) {
            noJumpForest.insert(std::move(std::make_pair(tree.first, IRTranslate::NoJumpTree(tree.second))));
        }

        IRTranslate::IRLinearForest reblocked;
        for(auto& tree: noJumpForest) {
            reblocked.insert(std::make_pair(tree.first, tree.second.BuildTree()));
        }

        IRTranslate::IRPrinter printerReblokced("reblocked_IR.dot");
        printerReblokced.CreateGraph(reblocked);

        RegLifecycle::LifecyclesList graphs;

        RegLifecycle::LifecyclePrinter printerLifecycle("lifecycle.dot");
        printerLifecycle.PrintPrefix();
        for(auto& trees: reblocked) {
            CodeGeneration::Muncher muncher(trees.second);
            auto list = muncher.CreateInstractionsList();
            std::cout << trees.first->GetString() << std::endl;
            std::cout << "-------------------------" << std::endl;
            RegLifecycle::LifecycleGraph lifecycleGraph(list);
            lifecycleGraph.BuildLifecycle();
            printerLifecycle.Print(lifecycleGraph.GetNodesList());
            for(auto& l: list.Instructions) {
                std::cout  << l->FormatLong() << std::endl;
            }
            std::cout << std::endl;
        }
        printerLifecycle.PrintPostfix();
      } catch(SymbolTable::DeclarationException e) {
        std::cout << NF_RED << "Declaration error: " << e.what() << NF_RESET << std::endl;
        return 1;
      }
  }
  return 0;
}
