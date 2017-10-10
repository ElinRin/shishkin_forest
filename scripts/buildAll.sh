#!/bin/bash
./scripts/generateTokenHeader.sh && flex lexer.lex && gcc -g -c lex.yy.c -o lex.o && g++ -g -c parser.cpp -Wno-write-strings -o parser.o && g++ -g -o parser lex.o parser.o -lfl
