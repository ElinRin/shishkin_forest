CFLAGS = -ISyntaxTree

all: parser

parser: lex.o parser.o treeNode.o nodes.o
	g++ -g -o parser treeNode.o nodes.o lex.o parser.o -lfl $(CFLAGS)

parser.o:  spec.yy
	bison -d --verbose spec.yy && mv spec.tab.hh tokens.h && mv spec.tab.cc parser.cpp && g++ -g -c --std=c++14 parser.cpp -Wno-write-strings -o parser.o $(CFLAGS)

lex.o: lexer.lex
	flex lexer.lex && gcc -g -c lex.yy.c -o lex.o $(CFLAGS)

nodes.o: SyntaxTree/AcceptVisitor.cpp
	g++ -g -c SyntaxTree/AcceptVisitor.cpp -o nodes.o $(CFLAGS)

treeNode.o: SyntaxTree/TreeNode.cpp
		g++ -g -c SyntaxTree/TreeNode.cpp -o treeNode.o $(CFLAGS)

clean:
	rm treeNode.o nodes.o lex.o parser.o parser
