#!/bin/bash
make all && cat $1 | ./buildIR && dot -Tsvg IR.dot -o IR.svg && dot -Tsvg can_IR.dot -o can_IR.svg
