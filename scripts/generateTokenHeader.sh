#!/bin/bash
bison -d --verbose spec.y && mv spec.tab.h tokens.h && mv spec.tab.c parser.c
