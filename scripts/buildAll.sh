#!/bin/bash
./scripts/generateTokenHeader.sh && flex lexer.lex && gcc -g -c lex.yy.c -o lex.o && gcc -g -c parser.c -o parser.o && gcc -g -o parser lex.o parser.o -lfl
