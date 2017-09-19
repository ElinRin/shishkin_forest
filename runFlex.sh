#!/bin/bash
flex $1 && gcc lex.yy.c -lfl && ./a.out
