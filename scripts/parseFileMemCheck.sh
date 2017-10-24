#!/bin/bash
make all && cat $1 |   valgrind --leak-check=full -v ./parser && dot -Tsvg tree.dot -o tree.svg
