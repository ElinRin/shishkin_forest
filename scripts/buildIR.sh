#!/bin/bash
time cat $1 | ./buildIR && echo Processing finished && dot -Tsvg IR.dot -o IR.svg && dot -Tsvg can_IR.dot -o can_IR.svg && dot -Tsvg lin_IR.dot -o lin_IR.svg && dot -Tsvg reblocked_IR.dot -o reblocked_IR.svg && dot -Tsvg lifecycle.dot -o lifecycle.svg && dot -Tsvg variables.dot -o variables.svg
