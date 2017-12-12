CFLAGS = -ISymbolTable -ISyntaxTree -I. -ITypeChecker -IActivationRecords -ITestActivationRecords --std=c++1z
SYMBOL_TABLE_SRC_DIR = SymbolTable
SYMBOL_TABLE_SRC = $(wildcard $(SYMBOL_TABLE_SRC_DIR)/*.cpp)

SYMBOL_TABLE_OBJ = $(SYMBOL_TABLE_SRC:$(SYMBOL_TABLE_SRC_DIR)/%.cpp=./%.o)

TEST_SYMBOL_TABLE_SRC_DIR = TestSymbolTable
TEST_SYMBOL_TABLE_SRC = $(wildcard $(TEST_SYMBOL_TABLE_SRC_DIR)/*.cpp)
TEST_SYMBOL_TABLE_OBJ = $(TEST_SYMBOL_TABLE_SRC:$(TEST_SYMBOL_TABLE_SRC_DIR)/%.cpp=./%.o)

PARSER_OBJ = parser.o lex.o treeNode.o nodes.o prettyPrint.o 

TYPE_CHECKER_SRC_DIR = TypeChecker
TYPE_CHECKER_SRC = $(wildcard $(TYPE_CHECKER_SRC_DIR)/*.cpp)
TYPE_CHECKER_OBJ = $(TYPE_CHECKER_SRC:$(TYPE_CHECKER_SRC_DIR)/%.cpp=./%.o)

TEST_AR_SRC_DIR = TestActivationRecords
TEST_AR_SRC = $(wildcard $(TEST_AR_SRC_DIR)/*.cpp)
TEST_AR_OBJ = $(TEST_AR_SRC:$(TEST_AR_SRC_DIR)/%.cpp=./%.o)

AR_SRC_DIR = ActivationRecords
AR_SRC = $(wildcard $(AR_SRC_DIR)/*.cpp)
AR_OBJ = $(AR_SRC:$(AR_SRC_DIR)/%.cpp=./%.o)


all: graph testTableListClasses checkTypes listActivations
		
listActivations: $(SYMBOL_TABLE_OBJ) $(PARSER_OBJ) $(TYPE_CHECKER_OBJ) listActivations.o $(AR_OBJ) $(TEST_AR_OBJ)
	g++ -g -o listActivations $(SYMBOL_TABLE_OBJ) $(PARSER_OBJ) $(TYPE_CHECKER_OBJ) listActivations.o $(AR_OBJ) $(TEST_AR_OBJ) $(CFLAGS) -lfl

listActivations.o: ListActivations.cpp
	g++ -g -c ListActivations.cpp -o listActivations.o $(CFLAGS)

checkTypes: $(SYMBOL_TABLE_OBJ) $(PARSER_OBJ) $(TYPE_CHECKER_OBJ) checkTypes.o
	g++ -g -o checkTypes $(SYMBOL_TABLE_OBJ) checkTypes.o $(PARSER_OBJ) $(TYPE_CHECKER_OBJ) $(CFLAGS) -lfl

checkTypes.o: CheckTypes.cpp
	g++ -g -c CheckTypes.cpp -o checkTypes.o $(CFLAGS)

testTableListClasses: $(SYMBOL_TABLE_OBJ) $(TEST_SYMBOL_TABLE_OBJ) $(PARSER_OBJ) tableTest.o
	g++ -g -o testTableListClasses $(SYMBOL_TABLE_OBJ) $(TEST_SYMBOL_TABLE_OBJ) $(PARSER_OBJ) tableTest.o $(CFLAGS) -lfl

tableTest.o: tableTest.cpp
	g++ -g -c tableTest.cpp -o tableTest.o $(CFLAGS)

graph: $(PARSER_OBJ) graph.o
	g++ -g -o graph $(PARSER_OBJ) graph.o -lfl $(CFLAGS)

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

./%.o: $(TYPE_CHECKER_SRC_DIR)/%.cpp
	g++ -g -c $< -o $@ $(CFLAGS)
	
./%.o: $(AR_SRC_DIR)/%.cpp
	g++ -g -c $< -o $@ $(CFLAGS)

./%.o: $(TEST_AR_SRC_DIR)/%.cpp
	g++ -g -c $< -o $@ $(CFLAGS)

prettyPrint.o: PrettyPrint/PrintVisitor.cpp
		g++ -g -c PrettyPrint/PrintVisitor.cpp -o prettyPrint.o $(CFLAGS)

clean:
	rm checkTypes checkTypes.o $(SYMBOL_TABLE_OBJ) $(TEST_SYMBOL_TABLE_OBJ) $(TYPE_CHECKER_OBJ) treeNode.o nodes.o lex.o parser.o graph.o graph
