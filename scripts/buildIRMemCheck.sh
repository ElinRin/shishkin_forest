#!/bin/bash
make all && cat $1 |   valgrind --leak-check=full ./buildIR && dot -Tsvg IR.dot -o IR.svg
