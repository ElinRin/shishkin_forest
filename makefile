CFLAGS = -ISymbolTable -ISyntaxTree --std=c++1z
SYMBOL_TABLE_SRC_DIR = SymbolTable
SYMBOL_TABLE_SRC = $(wildcard $(SYMBOL_TABLE_SRC_DIR)/*.cpp)

SYMBOL_TABLE_OBJ = $(SYMBOL_TABLE_SRC:$(SYMBOL_TABLE_SRC_DIR)/%.cpp=./%.o)

all: parser $(SYMBOL_TABLE_OBJ)

parser: parser.o lex.o treeNode.o nodes.o prettyPrint.o
	g++ -g -o parser treeNode.o nodes.o lex.o parser.o prettyPrint.o -lfl $(CFLAGS)

parser.o:  spec.yy
	bison -d --verbose spec.yy && mv spec.tab.hh tokens.h && mv spec.tab.cc parser.cpp && g++ -g -c parser.cpp -Wno-write-strings -o parser.o $(CFLAGS)

lex.o: lexer.lex
	flex++ lexer.lex && g++ -g -c lex.yy.cc -o lex.o $(CFLAGS)

nodes.o: SyntaxTree/AcceptVisitor.cpp
	g++ -g -c SyntaxTree/AcceptVisitor.cpp -o nodes.o $(CFLAGS)

treeNode.o: SyntaxTree/TreeNode.cpp
	g++ -g -c SyntaxTree/TreeNode.cpp -o treeNode.o $(CFLAGS)
		
./%.o: $(SYMBOL_TABLE_SRC_DIR)/%.cpp
	g++ -g -c $< -o $@

prettyPrint.o: PrettyPrint/PrintVisitor.cpp
		g++ -g -c PrettyPrint/PrintVisitor.cpp -o prettyPrint.o $(CFLAGS)

clean:
	rm treeNode.o nodes.o lex.o parser.o parser
