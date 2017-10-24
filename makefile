CFLAGS = -ISyntaxTree --std=c++1z

all: parser

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

prettyPrint.o: PrettyPrint/PrintVisitor.cpp
		g++ -g -c PrettyPrint/PrintVisitor.cpp -o prettyPrint.o $(CFLAGS)

clean:
	rm treeNode.o nodes.o lex.o parser.o parser
