#!/bin/bash
cat $1 |   valgrind --leak-check=full -v ./graph && dot -Tsvg tree.dot -o tree.svg
