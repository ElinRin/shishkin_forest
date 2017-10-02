#!/bin/bash
bison -d spec.y && mv spec.tab.h tokens.h && mv spec.tab.c parser.c
