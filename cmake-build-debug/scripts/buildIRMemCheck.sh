#!/bin/bash
cat $1 |   valgrind --leak-check=full ./buildIR
