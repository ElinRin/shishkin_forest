#!/bin/bash
flex $1 && gcc lex.yy.c -ll && ./a.out
