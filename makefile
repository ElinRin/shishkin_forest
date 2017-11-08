CFLAGS = -ISymbolTable -ISyntaxTree --std=c++1z
SYMBOL_TABLE_SRC_DIR = SymbolTable
SYMBOL_TABLE_SRC = $(wildcard $(SYMBOL_TABLE_SRC_DIR)/*.cpp)

SYMBOL_TABLE_OBJ = $(SYMBOL_TABLE_SRC:$(SYMBOL_TABLE_SRC_DIR)/%.cpp=./%.o)

TEST_SYMBOL_TABLE_SRC_DIR = TestSymbolTable
TEST_SYMBOL_TABLE_SRC = $(wildcard $(TEST_SYMBOL_TABLE_SRC_DIR)/*.cpp)

TEST_SYMBOL_TABLE_OBJ = $(TEST_SYMBOL_TABLE_SRC:$(TEST_SYMBOL_TABLE_SRC_DIR)/%.cpp=./%.o)

all: graph testTableListClasses

testTableListClasses: $(SYMBOL_TABLE_OBJ) $(TEST_SYMBOL_TABLE_OBJ) parser.o lex.o treeNode.o nodes.o prettyPrint.o tableTest.o
	g++ -g -o testTableListClasses $(SYMBOL_TABLE_OBJ) $(TEST_SYMBOL_TABLE_OBJ) parser.o lex.o treeNode.o nodes.o prettyPrint.o tableTest.o $(CFLAGS) -lfl

tableTest.o: tableTest.cpp
	g++ -g -c tableTest.cpp -o tableTest.o $(CFLAGS)

graph: parser.o lex.o treeNode.o nodes.o prettyPrint.o graph.o
	g++ -g -o graph treeNode.o nodes.o lex.o parser.o prettyPrint.o graph.o -lfl $(CFLAGS)

graph.o: graph.cpp
	g++ -g -c graph.cpp -o graph.o $(CFLAGS)

parser.o:  spec.yy
	bison -d --verbose spec.yy && mv spec.tab.hh tokens.h && mv spec.tab.cc parser.cpp && g++ -g -c parser.cpp -Wno-write-strings -o parser.o $(CFLAGS)

lex.o: lexer.lex
	flex++ lexer.lex && g++ -g -c lex.yy.cc -o lex.o $(CFLAGS)

nodes.o: SyntaxTree/AcceptVisitor.cpp
	g++ -g -c SyntaxTree/AcceptVisitor.cpp -o nodes.o $(CFLAGS)

treeNode.o: SyntaxTree/TreeNode.cpp
	g++ -g -c SyntaxTree/TreeNode.cpp -o treeNode.o $(CFLAGS)
		
./%.o: $(SYMBOL_TABLE_SRC_DIR)/%.cpp
	g++ -g -c $< -o $@ $(CFLAGS)

./%.o: $(TEST_SYMBOL_TABLE_SRC_DIR)/%.cpp
	g++ -g -c $< -o $@ $(CFLAGS)

prettyPrint.o: PrettyPrint/PrintVisitor.cpp
		g++ -g -c PrettyPrint/PrintVisitor.cpp -o prettyPrint.o $(CFLAGS)

clean:
	rm $(SYMBOL_TABLE_OBJ) treeNode.o nodes.o lex.o parser.o grpah.o graph test
