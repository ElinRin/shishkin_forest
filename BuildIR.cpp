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
#include "LifecyclePrinter.h"
#include "VariableGraphPrinter.h"

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

        RegLifecycle::VariableGraphPrinter printerVariableGraph("variables.dot");
        printerVariableGraph.PrintPrefix();
        for(auto& trees: reblocked) {
            CodeGeneration::Muncher muncher(trees.second);
            auto list = muncher.CreateInstractionsList();
            std::cout << trees.first->GetString() << std::endl;
            std::cout << "-------------------------" << std::endl;

            int iteration = 0;
            while (true) {
                RegLifecycle::LifecycleGraph lifecycleGraph(list);
                lifecycleGraph.BuildLifecycle();

                RegLifecycle::VariableGraph variablesGraph(lifecycleGraph);

                const RegLifecycle::VariableGraph::INode* badNode = variablesGraph.GetBadNode();

                if (badNode == nullptr) {
                    printerLifecycle.Print(lifecycleGraph.GetNodesList());
                    printerVariableGraph.Print(variablesGraph);
                    break;
                } else {
                    ++iteration;
                    std::cout << "ITERATION" << std::endl;
                    list.Registers.push_back(std::make_unique<const IR::Temp>(
                            "Additional stack: " + std::to_string(iteration)));
                    const IR::Temp* stackRegister = list.Registers.back().get();
                    int tempRead = 0;
                    int tempPush = 0;
                    for (int i = 0; i < list.Instructions.size(); ++i) {
                        for (const IR::Temp* var : list.Instructions[i]->UsedVars()) {
                            if ((*var) == badNode->GetReg()) {
                                list.Instructions[i]->RemoveUsed(var);
                                list.Registers.push_back(std::make_unique<const IR::Temp>(
                                        "Temp read from stack: " + std::to_string(tempRead)));
                                ++tempRead;
                                list.Instructions[i]->AddUsed(list.Registers.back().get());

                                list.Instructions.insert(list.Instructions.begin() + i,
                                                         std::make_unique<X86::RegMove>("MOV %0 [%1]",
                                                                      stackRegister,
                                                                      list.Registers.back().get()));
                                ++i;
                                break;
                            }
                        }
                        for (auto& var : list.Instructions[i]->DefinedVars()) {
                            if ((*var) == badNode->GetReg()) {
                                list.Instructions[i]->RemoveDefined(var);
                                list.Registers.push_back(std::make_unique<const IR::Temp>(
                                        "Temp push to stack: " + std::to_string(tempPush)));
                                ++tempPush;
                                list.Instructions[i]->AddDefined(list.Registers.back().get());

                                list.Instructions.insert(list.Instructions.begin() + i + 1,
                                                         std::make_unique<X86::RegMove>("MOV %0 [%1]",
                                                                                        list.Registers.back().get(),
                                                                                        stackRegister));
                                ++i;
                                break;
                            }
                        }
                    }
                }
            }
            for(auto& l: list.Instructions) {
                std::cout  << l->FormatLong() << std::endl;
            }
            std::cout << std::endl;
        }
        printerVariableGraph.PrintPostfix();
        printerLifecycle.PrintPostfix();
      } catch(SymbolTable::DeclarationException e) {
        std::cout << NF_RED << "Declaration error: " << e.what() << NF_RESET << std::endl;
        return 1;
      }
  }
  return 0;
}
