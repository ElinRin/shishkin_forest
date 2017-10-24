#!/bin/bash
make all && cat $1 | ./parser && dot -Tsvg tree.dot -o tree.svg
