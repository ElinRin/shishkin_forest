#!/bin/bash
cat $1 |   valgrind --leak-check=full ./buildIR && dot -Tsvg IR.dot -o IR.svg && dot -Tsvg can_IR.dot -o can_IR.svg && dot -Tsvg lin_IR.dot -o lin_IR.svg
